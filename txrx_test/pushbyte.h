#ifndef PUSHBYTE_H
#define PUSHBYTE_H

#include <stdint.h>
#include <assert.h>

/* A PushByte is like a buffer for bits of
 * size 8; you "push" bits into it, LSB first,
 * and after 8 iterations you get a whole byte back.
 */

class PushByte {
public:
	PushByte() {
		clear();
	}
	
	void clear() {
		byte = 0;
		idx = 0;
	}
	
	void push(bool bit) {
		uint8_t bitVal = bit ? 1 : 0;
		byte |= (bitVal << (idx++));
	}
	
	bool isFull() const {
		return idx == 8;
	}
	
	uint8_t value() const {
		assert(isFull());
		return byte;
	}
	
//private:
	uint8_t byte;
	uint8_t idx;
};

#endif
