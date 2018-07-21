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

	bool static readPin(uint8_t port, uint8_t pin) {
		return (PIND & (1 << pin));
	}

	bool rxRead(uint8_t port, uitn8_t pin) {
		return readPin(RX_PORT, RX_DATA_PIN);
	}

	void txEnable() {
		setPin(TX_PORT, TX_ENABLE_PIN, 1);
	}

	void txDisable() {
		setPin(TX_PORT, TX_ENABLE_PIN, 0);
	}
}
