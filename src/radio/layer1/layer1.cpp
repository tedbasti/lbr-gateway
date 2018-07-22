
#include "../layer1/layer1.h"
#include "../layer2/layer2.h"

#include "../bitringbuffer.h"
#include "../../util/configExtern.h"
#include "../../usart/usart.h"
#include "../../util/io.h"
#include <util/delay.h>

#define START_SEQUENCE 0xE3

namespace MAIN {
	extern BitRingBuffer<RECEIVE_BUFFER_SIZE> receiveBuffer;
}

namespace LAYER1 {
	static BitRingBuffer<RECEIVE_BUFFER_SIZE> ringBuf;

	static volatile uint8_t sendOffsetCounter = 0;
	static volatile uint8_t receiveOffsetCounter = 0;
	static volatile uint8_t receiveOffset = MAX_OFFSET_COUNTER/2;

	bool sendBit(bool bit) {
		if(ringBuf.isFull()) {
			DEBUG_PRINT('S');
			return false;
		}
		ringBuf.pushBit(bit);
		return true;
	}

	void init() {

	}

	bool onTimeTransmit() {
		++sendOffsetCounter;

		if (sendOffsetCounter == MAX_OFFSET_COUNTER) {
			sendOffsetCounter = 0;
			if (ringBuf.isEmpty()) {
				IO::txVCCDisable();
				return true;
			}
			bool outBit = ringBuf.popBit();
			IO::txWrite(outBit);
			return true;
		} else {
			return false;
		}
	}

	void onTimeReceive() {
		static bool synchronizationActive=false;
		static bool dataBit;
		++receiveOffsetCounter;
		if (receiveOffsetCounter == receiveOffset) {
			if(MAIN::receiveBuffer.isFull()) {
				DEBUG_PRINT('R');
				return;
			}
			dataBit = IO::rxRead();
			MAIN::receiveBuffer.pushBit(dataBit);
			synchronizationActive=true;
			/**
			 * Is ok, because receiveOffset
			 * is always MAX_OFFSET_COUNTER/2!
			 */
			return;
		}

		if (synchronizationActive) {
			if(IO::rxRead() != dataBit) {
				synchronizationActive = false;
				receiveOffsetCounter = 0;
			}
		}

		if(receiveOffsetCounter == MAX_OFFSET_COUNTER) {
			receiveOffsetCounter = 0;
		}
	}

	uint16_t getTXBufferSpace() {
		return ringBuf.remainingSpace();
	}
}
