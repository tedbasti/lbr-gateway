#ifndef BITRINGBUFFER_H
#define BITRINGBUFFER_H

#include <stdint.h>
#include <assert.h>

/* The bitringbuffer is a simple ring buffer structure
 * for bits.
 */

typedef uint16_t BufferSize;

template<BufferSize Size>
class BitRingBuffer {
public:
	BitRingBuffer() {
		writeIdx = 0;
		readIdx = 0;
		space = Size;
	}
	
	bool isFull() const {
		return space == 0;
	}
	
	bool isEmpty() {
		return space == Size;
	}
	
	/* Remaining space for pushing, in bits */
	BufferSize remainingSpace() const {
		return space;
	}
	
	/* Enqueues a bit */
	void pushBit(bool bit) {
		assert(!isFull());
		
		uint8_t mask;
		uint8_t &byte = resolveBit(writeIdx, mask);
		
		if (bit) {
			byte |= mask;
		} else {
			byte &= ~mask;
		}
		
		writeIdx = (writeIdx + 1) % Size;
		--space;
	}
	
	/* Dequeues a bit */
	bool popBit() {
		assert(!isEmpty());
		
		uint8_t mask;
		uint8_t &byte = resolveBit(readIdx, mask);
		bool result = byte & mask;
		
		readIdx = (readIdx + 1) % Size;
		++space;
		
		return result;
	}
	
private:
	/* Returns reference to the byte containing bit at 'idx',
	 * and the corresponding bitmask */
	uint8_t &resolveBit(BufferSize idx, uint8_t &outMask) {
		uint8_t *byte = &data[idx / 8];
		uint8_t bitIdx = idx % 8;
		outMask = 1 << bitIdx;
		
		return *byte;
	}

	uint8_t data[Size/8];
	volatile BufferSize readIdx;
	volatile BufferSize writeIdx;
	volatile BufferSize space;
};

#endif
