
#include "../layer2/layer2.h"
#include "../layer1/layer1.h"

namespace LAYER2 {


	void receiveBit(bool bit) {
		//TODO: implement
	}

	void transmitData(uint8_t receiverID, char* data, const uint8_t len){
		//TODO: implement
	}

	static void encode(const char *data) {
		for(const char *p = data; *p; ++p) {
			manchester(*p);
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

	inline static void transmitToLayer1(bool bit) {
		Layer1::sendBit(bit);
	}

}
