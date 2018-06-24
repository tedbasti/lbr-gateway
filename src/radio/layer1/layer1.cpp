/*
 * layer1.cpp
 *
 *  Created on: Jun 23, 2018
 *      Author: ted
 */

#include "../layer1/layer1.h"
#include "../layer2/layer2.h"

#include "../bitringbuffer.h"
#include "../checksum.h"
#include "../../util/util.h"

namespace LAYER1 {
static BitRingBuffer<1024> ringBuf;

inline void sendBit(bool bit) {
	ringBuf.pushBit(bit);
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
	LAYER2::rcvBit(dataBit);
}

}
