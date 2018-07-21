/*
 * io.cpp
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#include <stdint>
#include "io.h"
#include "ioconfig.h"


namespace IO {
	void static setPin(uint8_t port, uint8_t pin, uint8_t value) {
		if(value != 0) {
			port |= (1 << pin);
		}
		else {
			port &= ~(1 << pin);
		}
	}

	void rxEnable() {
		setPin(RX_PORT_OUT, RX_ENABLE_PIN, 1);
	}

	void rxDisable() {
		setPin(RX_PORT_OUT, RX_ENABLE_PIN, 0);
	}
}
