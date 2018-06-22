#ifndef ENCODING_H
#define ENCODING_H

#include <stdint.h>
#include "bitringbuffer.h"
#include "pushbyte.h"

/* The Encoding namespace encapsules every aspect of how
 * raw data is transformed for on-the-wire (or air) transmission;
 * it handles start/stop sequences for sending frames and
 * the manchester encoding for actual data bits */

namespace Encoding {
	
const uint8_t startSeq = 0xE3; // 11100011
const uint8_t endSeq = 0xFF; // 11111111

const uint8_t startEndSeqSize = 2*8;

inline uint16_t encodedByteSize(uint8_t bytes) {
	/* Mancherster: 2 bits encode 1 bit */
	return bytes * 16;
}

inline bool isStartSeq(uint8_t seq) {
	return seq == startSeq;
}

inline bool isEndSeq(uint8_t seq) {
	return (seq & 0x3) == 0x3;
}

template<BufferSize Size>
void pushNibbleReverse(BitRingBuffer<Size> &buf, uint8_t nibble) {
	for (uint8_t i = 0; i < 4; ++i) {
		buf.pushBit(nibble & (1 << (4-1-i)));
	}
}

template<BufferSize Size>
void pushByteReverse(BitRingBuffer<Size> &buf, uint8_t byte) {
	for (uint8_t i = 0; i < 8; ++i) {
		buf.pushBit(byte & (1 << (8-1-i)));
	}
}

/* These functions don't check remaining buffer space
 * as they assume the user has checked that the buffer
 * has space for at least one whole packet */
template<BufferSize Size>
void pushStartSeq(BitRingBuffer<Size> &buf) {
	pushByteReverse(buf, startSeq);
}

template<BufferSize Size>
void pushEndSeq(BitRingBuffer<Size> &buf) {
	/* End seq length is only 2;
	 * push a few more bits just be sure */
	buf.pushBit(true);
	buf.pushBit(true);
	buf.pushBit(true);
	buf.pushBit(true);
}

template<BufferSize Size>
void pushBit(BitRingBuffer<Size> &buf, bool bit) {
	if (bit) {
		/* 1 -> low high */
		buf.pushBit(false);
		buf.pushBit(true);
	} else {
		/* 0 -> high low */
		buf.pushBit(true);
		buf.pushBit(false);
	}
}

template<BufferSize Size>
void pushByte(BitRingBuffer<Size> &buf, uint8_t byte) {
	for (uint8_t i = 0; i < 8; ++i) {
		pushBit(buf, byte & (1 << i));
	}
}

template<BufferSize Size>
void pushBytes(BitRingBuffer<Size> &buf, uint8_t const *bytes, uint8_t count) {
	for (uint8_t i = 0; i < count; ++i) {
		pushByte(buf, bytes[i]);
	}
}

template<BufferSize Size>
void pushString(BitRingBuffer<Size> &buf, char const *str) {
	for (char const *p = str; *p; ++p) {
		pushByte(buf, static_cast<uint8_t>(*p));
	}
}

/* The ByteDecoder is like a PushByte (see pushbyte.h),
 * except that you push manchester-encoded bits directly
 * from the wire, and get a decoded byte back eventually.
 */
 
class ByteDecoder {
public:
	ByteDecoder() {
		tick = true;
		lastBit=false;
	}

	bool hasFullByte() const {
		return pushByte.isFull();
	}
	
	/* Get fully decoded byte */
	uint8_t value() const {
		return pushByte.value();
	}
	
	/* Reset state */
	void clear() {
		pushByte.clear();
		tick = true;
	}
	
	/* Returns false if invalid sequence was encountered */
	bool pushEncodedBit(bool bit) {
		if (tick) {
			/* tick */
			tick = false;
			lastBit = bit;
		} else {
			/* tock */
			tick = true;
			if (lastBit != bit) {
				pushByte.push(bit);
			} else {
				/* No flank -> invalid manchester sequence */
				return false;
			}
		}
		
		return true;
	}
private:
	PushByte pushByte;
	/* tick is true on first manchester bit */
	bool tick;
	/* Previous (tick) encoded bit */
	bool lastBit;
};

}

#endif
