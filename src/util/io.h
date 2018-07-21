/*
 * io.h
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#ifndef SRC_IO_H_
#define SRC_IO_H_


namespace IO {
	void init();

	bool rxRead();

	void txVCCEnable();

	void txVCCDisable();

	void txWrite(bool value);

	void ledTXWrite(bool value);

	void ledErrorWrite(bool value);
}


#endif /* SRC_IO_H_ */
