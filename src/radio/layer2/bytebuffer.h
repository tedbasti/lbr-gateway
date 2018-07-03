/*
 * Within that ByteBuffer you can store multiple bytes.
 * After you store them, you can get the raw data and
 * copy them where you want.
 * Copy could be done with
 */

#ifndef BYTEBUFFER
#define BYTEBUFFER

#include <stdint.h>
//string.h needed for memcpy
#include <string.h>
//Bitringbuffer need for the typedef of BufferSize
#include "../bitringbuffer.h"

template <BufferSize Size>
class ByteBuffer {
private:
	uint8_t buffer[Size];
	//The current fill state of the buffer
	uint16_t fill=0;

public:
	void pushByte(uint8_t byte) {
		assert(fill < Size);
		buffer[fill] = byte;
		fill++;
	}

	uint8_t popByte() {
		assert(fill > 0);
		fill--;
		return buffer[fill];
	}

	void clear() {
		fill = 0;
	}

	uint8_t getFill() {
		return fill;
	}

	void rawValue(uint8_t* result, uint8_t skip=0) {
		memcpy(result, buffer+skip, fill);
		fill = skip;
	}
};

#endif
