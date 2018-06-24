/*
 * layer1.cpp
 *
 *  Created on: Jun 23, 2018
 *      Author: ted
 */

#include "../layer1/layer1.h"

#include "../bitringbuffer.h"
#include "../checksum.h"
#include "../../util/util.h"

namespace LAYER1 {
static BitRingBuffer<1024> ringBuf;

void sendBit(bool bit) {
	ringBuf.pushBit(bit);
}

void init() {

}

void onTimeSlice() {
	if (ringBuf.isEmpty()) {
		return;
	}

	bool outBit = ringBuf.popBit();
	SET_P(DATA_OUT_PORT, DATA_OUT_PIN, outBit);
}

}
