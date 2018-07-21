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
}
