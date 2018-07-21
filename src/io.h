/*
 * io.h
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#ifndef SRC_IO_H_
#define SRC_IO_H_


#include <stdint>

namespace IO {
	void static setPin(uint8_t port, uint8_t pin, uint8_t value);

	bool static readPin(uint8_t port, uint8_t pin);

	void init();

	bool rxRead();

	void txEnable();

	void txDisable();

	void txWrite(bool value);

	void ledTXWrite(bool value);

	void ledErrorWrite(bool value);
}


#endif /* SRC_IO_H_ */
