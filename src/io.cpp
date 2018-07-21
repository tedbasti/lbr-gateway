/*
 * io.cpp
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#include "io.h"


namespace IO {
	void static setPin(uint8_t port, uint8_t pin, uint8_t value) {
		if(value != 0) {
			port |= (1 << pin);
		}
		else {
			port &= ~(1 << pin);
		}
	}
}
