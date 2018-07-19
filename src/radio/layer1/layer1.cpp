
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

	bool sendBit(bool bit) {
		if(ringBuf.isFull()) {
			return false;
		}
		ringBuf.pushBit(bit);
		return true;
	}

	void init() {

	}

	void onTimeTransmit() {
		if (ringBuf.isEmpty()) {
			return;
		}

		bool outBit = ringBuf.popBit();
		SET_P(DATA_OUT_PORT, DATA_OUT_PIN, outBit);
	}

	void onTimeReceive() {
		uint8_t dataBit = DATA_IN ? 1 : 0;
		MAIN::receiveBuffer.pushBit(dataBit);
	}

	uint16_t getTXBufferSpace() {
		return ringBuf.remainingSpace();
	}
}
