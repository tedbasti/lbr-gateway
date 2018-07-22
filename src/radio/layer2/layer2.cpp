
#include <util/delay.h>
#include "layer2.h"
#include "../layer1/layer1.h"
#include "frame.h"
#include "checksum.h"
#include "bytebuffer.h"
#include "../../usart/usart.h"
#include "../../util/configExtern.h"
#include "../../util/io.h"

namespace LAYER2 {
	typedef void (*nextLayerFunction)(const uint8_t *data, uint8_t len);
	nextLayerFunction volatile higherLayer = USART::transmit;
	typedef XorChecksum8 ChecksumAlgo;
	const uint8_t startSeq = 0xE3; // 11100011
	const uint8_t endSeq = 0xFE; // 11111110
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
					DEBUG_PRINT('1');
					/*
					 * Ensure that at least 5 bytes are within the buffer
					 * receiver, sender, payloadlen, payload, checksum
					 * TODO: Make 4 and 10 more generic (with some defines)
					 */
					if(buffer.getFill() > 3 && buffer.getFill() < 10) {
						DEBUG_PRINT('2');
						//Get the checksum
						uint8_t checksumReceived = buffer.popByte();
						//Generate a frame
						Frame f;
						//Calculate the payloadLen
						f.payloadLen = buffer.getFill()-2;
						/*
						 * The second parameter is what to skip.
						 * In this case only receiver and
						 * sender needs to be skipped
						 */
						buffer.rawValue(f.payload, 2);
						f.sender = buffer.popByte();
						f.receiver = buffer.popByte();
						ChecksumAlgo checkSum;
						checkSum.addByte(f.receiver);
						checkSum.addByte(f.sender);
						checkSum.addByte(f.payloadLen);
						checkSum.addBytes(f.payload, f.payloadLen);
#ifdef DEBUG_ENABLE
						if (checkSum.getDigest() == checksumReceived) {
							DEBUG_PRINT('3');
						}
						if (f.receiver == CONFIG::senderId) {
							DEBUG_PRINT('4');
						}
#endif

						/*
						 * f.receiver == CONFIG::senderId may look strange,
						 * but the initial sender puts the receiverId.
						 * This is for the receiver his configured senderId.
						 * TODO: Maybe switch names within the CONFIG stuff
						 */
						if (checkSum.getDigest() == checksumReceived && f.receiver == CONFIG::senderId) {
							DEBUG_PRINT('5');
							higherLayer(f.payload, f.payloadLen);
						}
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
			DEBUG_PRINT('0');
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
			bitmask = (bitmask >> 1);
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
		// Create frame
		ChecksumAlgo ckSum;
		ckSum.addByte(receiverID);
		ckSum.addByte(CONFIG::senderId);
		ckSum.addByte(len);
		ckSum.addBytes((const unsigned char*) data, len);

		USART::transmitChar('0');
		// Enable TX radio module VCC and wait one us
		IO::txVCCEnable();
		_delay_us(1);

		// Send 16 bits with manchester for synchronisazion!
		pushByteToLayer1_Encoded(0);

		pushByteToLayer1(startSeq);
		//Send the Receiver
		pushByteToLayer1_Encoded(receiverID);
		//Send the transmitter
		pushByteToLayer1_Encoded(CONFIG::senderId);
		//Send the data
		for(uint8_t i=0; i<len; i++) {
			pushByteToLayer1_Encoded(data[i]);
			USART::transmitChar(data[i]);
		}
		pushByteToLayer1_Encoded(ckSum.getDigest());
		//Send end sequence
		pushBitToLayer1(1);
		pushBitToLayer1(1);
		//Just a zero to deactivate sending
		pushBitToLayer1(0);
		USART::transmitChar('2');
		//pushByteToLayer1(0x00);
	}

	bool onHandlingNeeded(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer) {
		return (sendBufferEnoughSpace()) && (!transmitBuffer.isEmpty());
	}

	bool sendData(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer) {
		DataSet dSet = transmitBuffer.popFront();
		transmitData(CONFIG::receiverId, dSet.payload, CONFIG::payloadLen);
		return true;
	}

	bool sendBufferEnoughSpace() {
		bool x = (LAYER1::getTXBufferSpace() > MAX_BUFFER_SPACE_NEEDED_FOR_PACKAGE);
		if (!x) {
			USART::transmitChar('f');
		}
		return (LAYER1::getTXBufferSpace() > MAX_BUFFER_SPACE_NEEDED_FOR_PACKAGE);
	}
}
