
#include "layer2.h"
#include "../layer1/layer1.h"
#include "frame.h"
#include "checksum.h"
#include "bytebuffer.h"
#include "../../usart/usart.h"

namespace LAYER2 {
	typedef XorChecksum8 ChecksumAlgo;
	const uint8_t startSeq = 0xE3; // 11100011
	const uint8_t endSeq = 0xFF; // 11111111
	static uint8_t byte = 0;
	static uint8_t bitsReceived = 0;
	/*
	 * Within the ByteBuffer a Frame is stored.
	 * Not so much bytes are needed. 10 should work.
	 */
	static ByteBuffer<10> buffer;

	/**
	 * This is for the receive logic
	 */
	static bool inline decodeManchester(bool bit) {
		/*
		 * The byte will be stored within the pushByte,
		 * when 8 bits are done.
		 * bitsReceived store how much bits are received.
		 * When the bitsReceived is odd, (the second bit
		 * of a manchester decoding) it must differ
		 * from the current bit to be valid.
		 * Otherwise false is returned.
		 */
		static bool lastBit = 0;

		//When its a odd bit position
		if (bitsReceived%2) {
			/*
			 * Manchester is always different!
			 */
			if (bit == lastBit) {
				return false;
			}
			//The current value of byte will shift to the left
			byte=byte<<1;
			/*
			 * We now that its a valid manchester code,
			 * and the second one
			 * (lastBit first received, bit afterwards)
			 * is always the value.
			 * So we just get the new value with byte|bit.
			 */
			byte=byte|bit;
		}

		//Count bitsReceived, AFTER check if odd
		bitsReceived++;
		//16 bits are one byte for manchester (16/2 = 8bits)
		if (bitsReceived == 16) {
			buffer.pushByte(byte);
			byte=0;
			bitsReceived=0;
		}
		//lastBit is now bit for the next round
		lastBit=bit;
		return true;
	}

	/*
	 * This function should only be called with
	 * valid frame bits!
	 * This function decode the given bits
	 * and give them to the "next layer" which
	 * builds up a frame from the data.
	 */
	bool inline receiveBitDecode(bool bit) {
		return decodeManchester(bit);
	}

	/*
	 * This function takes one bit from layer1,
	 * This function ensure that the next function
	 * only gets bits from the real frame.
	 */
#define STATE_WAITING 0
#define STATE_RECEIVING 1
	void receiveBit(bool bit) {
		/* Initialize this to something that can never be shifted
		 * into a valid sequence with non-sequence bits */
		static uint8_t seq = 0x55;
		//Just add the new bit to the seq
		seq = (seq << 1) | bit;
		/*
		 * At the beginning the state is initialized.
		 * When a valid startsequence is received,
		 * the state changed to receive until
		 * the "decode layer" returns that there was
		 * wrong bit sequence.
		 */
		static uint8_t currentState = STATE_WAITING;

		if (currentState == STATE_RECEIVING) {
			//Invalid Code!
			if (!receiveBitDecode(bit)) {
				//"Poor" version for end sequence
				//Only check first bits
				if ((seq & 0x3) == 0x3) {
					//Get the checksum
					uint8_t checksum = buffer.popByte();
					//Generate a frame
					Frame f;
					buffer.rawValue((uint8_t *)&f);
					ChecksumAlgo checkSum;
					checkSum.addByte(f.receiver);
					checkSum.addByte(f.sender);
					checkSum.addByte(f.payloadLen);
					checkSum.addBytes(f.payload, f.payloadLen);
					/*
					 * TODO: At the moment sender is always 0.
					 * Adapt to some new cases.
					 */
					if (checkSum.isValid() && f.sender == 0) {
						USART::transmit(f.payload, f.payloadLen);
					}
				}
				currentState = STATE_WAITING;
			}
		}
		/*
		 * When a start sequence appears,
		 * just start from the beginning.
		 */
		if (seq==startSeq) {
			buffer.clear();
			byte=0;
			bitsReceived=0;
			currentState=STATE_RECEIVING;
		}
	}

	/**
	 * Here starts the transmit logic
	 */
	inline static void pushBitToLayer1(bool bit) {
		LAYER1::sendBit(bit);
	}

	inline static void pushByteToLayer1(unsigned char data) {
		// 0b10000000
		unsigned char bitmask = 128;
		// Get each bit from left to right
		while (bitmask != 0) {
			// Get bit
			bool bit = data & bitmask;
			pushBitToLayer1(bit);
		}
	}

	static void encodeManchester(unsigned char data) {
			// 0b10000000
			unsigned char bitmask = 128;

			// Get each bit from left to right
			while(bitmask != 0) {
				// Get bit
				bool bit = data & bitmask;

				// Push code word to layer 1
				if(bit) {
					pushBitToLayer1(0);
					pushBitToLayer1(1);
				}
				else {
					pushBitToLayer1(1);
					pushBitToLayer1(0);
				}

				// Shift bitmask to next bit
				bitmask = (bitmask >> 1);
			}
		}

	static void inline pushByteToLayer1_Encoded(const char data) {
		encodeManchester(data);
	}

	void transmitData(uint8_t receiverID, uint8_t* data, const uint8_t len){
		ChecksumAlgo ckSum;
		ckSum.addByte(receiverID);
		ckSum.addByte(0);	// TODO Sender ID
		ckSum.addByte(len);
		ckSum.addBytes((const unsigned char*) data, len);
		/* Send warmup bytes; these are apparently necessary
		 * for a reliable transmission */
		pushByteToLayer1_Encoded(0xF0);
		//Send start sequence
		pushByteToLayer1(startSeq);
		//Send the len
		pushByteToLayer1_Encoded(len);
		//Send the Receiver
		pushByteToLayer1_Encoded(receiverID);
		//Send the transmitter
		pushByteToLayer1_Encoded((char)0);
		//Send the data
		for(uint8_t i=0; i<len; i++) {
			pushByteToLayer1_Encoded(data[i]);
		}
		pushByteToLayer1_Encoded(ckSum.getDigest());
		//Send end sequence
		pushByteToLayer1(endSeq);
	}
}
