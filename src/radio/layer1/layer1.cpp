
#include "../layer1/layer1.h"
#include "../layer2/layer2.h"

#include "../bitringbuffer.h"
#include "../../util/util.h"
#include "../../util/configExtern.h"

namespace MAIN {
	extern BitRingBuffer<RECEIVE_BUFFER_SIZE> receiveBuffer;
}

namespace LAYER1 {
	static BitRingBuffer<RECEIVE_BUFFER_SIZE> ringBuf;
	static volatile uint8_t offsetCounter = 0;
	static volatile uint8_t receiveOffset = 20;

	bool sendBit(bool bit) {
		if(ringBuf.isFull()) {
			return false;
		}
		ringBuf.pushBit(bit);
		return true;
	}

	void init() {

	}

	bool onTimeTransmit() {
		if (offsetCounter == MAX_OFFSET_COUNTER) {
			offsetCounter=0;
			if (ringBuf.isEmpty()) {
				return true;
			}

			bool outBit = ringBuf.popBit();
			SET_P(DATA_OUT_PORT, DATA_OUT_PIN, outBit);
			return true;
		} else {
			return false;
		}
	}

	void onTimeReceive() {
		offsetCounter++;
		if (offsetCounter == receiveOffset) {
			if(MAIN::receiveBuffer.isFull()) {
				return;
			}
			uint8_t dataBit = DATA_IN ? 1 : 0;
			MAIN::receiveBuffer.pushBit(dataBit);
		}
	}

	uint16_t getTXBufferSpace() {
		return ringBuf.remainingSpace();
	}
}
