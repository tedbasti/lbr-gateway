
#include "../layer2/layer2.h"
#include "../layer1/layer1.h"
#include "frame.h"

namespace LAYER2 {
	const uint8_t startSeq = 0xE3; // 11100011
	const uint8_t endSeq = 0xFF; // 11111111

	void receiveBit(bool bit) {
		//TODO: implement
	}

	inline static void transmitToLayer1(bool bit) {
		LAYER1::sendBit(bit);
	}

	/*
	 * Sends raw data to layer1.
	 * Raw means no channel encoding.
	 * Important for start/endsequence.
	 */
	static void pushRaw(unsigned char data) {
		// 0b10000000
		unsigned char bitmask = 128;
		// Get each bit from left to right
		while (bitmask != 0) {
			// Get bit
			bool bit = data & bitmask;
			transmitToLayer1(bit);
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
					transmitToLayer1(0);
					transmitToLayer1(1);
				}
				else {
					transmitToLayer1(1);
					transmitToLayer1(0);
				}

				// Shift bitmask to next bit
				bitmask = (bitmask >> 1);
			}
		}

	static void inline encode(const char data) {
		LAYER2::manchester(data);
	}

	static void encode(const char *data) {
		for(const char *p = data; *p; ++p) {
			encode(*p);
		}
	}

	template <typename ChecksumAlgo>
	void transmitData(uint8_t receiverID, char* data, const uint8_t len){
		ChecksumAlgo ckSum;
		ckSum.addByte(len);
		ckSum.addByte(receiverID);
		ckSum.addByte(0);
		ckSum.addBytes(data, len);
		/* Send warmup bytes; these are apparently necessary
		 * for a reliable transmission */
		encode(0xF0);
		//Send start sequence
		pushRaw(startSeq);
		//Send the len
		encode(len);
		//Send the Receiver
		encode(receiverID);
		//Send the transmitter
		encode((char)0);
		//Send the data
		for(int i=0; i<len; i++) {
			encode(data[i]);
		}
		encode(ckSum.getDigest());
		//Send end sequence
		pushRaw(endSeq);
	}
}
