/*
 * io.h
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#ifndef SRC_IO_H_
#define SRC_IO_H_


#include <stdint>
#include "ioconfig.h"

namespace IO {
	void static setPin(uint8_t port, uint8_t pin, uint8_t value);

	bool static readPin(uint8_t port, uint8_t pin);

	bool rxRead(uint8_t port, uint8_t pin);

	void txEnable();

	void txDisable();

	void txWrite(bool value);
}


#endif /* SRC_IO_H_ */
