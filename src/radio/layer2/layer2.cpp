
#include "../layer2/layer2.h"
#include "../layer1/layer1.h"
#include "frame.h"
#include "checksum.h"

namespace LAYER2 {
	typedef XorChecksum8 ChecksumAlgo;
	const uint8_t startSeq = 0xE3; // 11100011
	const uint8_t endSeq = 0xFF; // 11111111

	void receiveBit(bool bit) {
		//TODO: implement
	}

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

	static void manchester(unsigned char data) {
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
		LAYER2::manchester(data);
	}

	void transmitData(uint8_t receiverID, char* data, const uint8_t len){
		ChecksumAlgo ckSum;
		ckSum.addByte(len);
		ckSum.addByte(receiverID);
		ckSum.addByte(0);
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
