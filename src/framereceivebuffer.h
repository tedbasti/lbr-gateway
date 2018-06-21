#ifndef FRAMERECEIVEBUFFER_H
#define FRAMERECEIVEBUFFER_H

#include "frame.h"
#include "checksum.h"
#include "encoding.h"

/* The Frame Receive Buffer handles everything from taking
 * raw encoded bits (meaning, the actual encoded data bits
 * _between_ start and stop sequence) and decoding them back
 * into a full frame, performing a checksum test on the fly.
 */

template<uint8_t Size, class ChecksumAlgo>
class FrameReceiveBuffer {
public:
	FrameReceiveBuffer() {
		dataIdx = 0;
	}
	
	/* Reset state */
	void clear() {
		checksum.reset();
		byteDec.clear();
		dataIdx = 0;
	}
	
	/* Returns false if invalid sequence was encountered */
	bool pushEncodedBit(bool bit) {
		if (!byteDec.pushEncodedBit(bit)) {
			return false;
		}
		
		if (!byteDec.hasFullByte()) {
			return true;
		}
		
		data[dataIdx++] = byteDec.value();
		assert(dataIdx <= Size);
		
		checksum.addByte(byteDec.value());
		byteDec.clear();
		
		return true;
	}
	
	/* Whether a valid frame has been received at this point */
	bool hasValidFrame() const {
		/* Must have valid checksum and at least
		 * sender id, receiver id, and checksum field
		 * (empty payload is possible for now) */
		return checksum.isValid() && (dataIdx >= 3);
	}
	
	/* Retreive decoded frame */
	void getFrame(Frame &frame) const {
		assert(hasValidFrame());
		frame.sender = data[0];
		frame.receiver = data[1];
		
		// XXX This is really bad, the payload pointer
		// points to our local buffer whose contents become
		// invalid on next 'clear()'.
		frame.payload = &data[2];
		frame.payloadLen = dataIdx - 3;
	}
	
	// For debug purposes only
	uint8_t const *rawFrameData(uint8_t &outLen) {
		outLen = dataIdx-1;
		return data;
	}
	
private:
	Encoding::ByteDecoder byteDec;
	ChecksumAlgo checksum;
	uint8_t data[Size];
	uint8_t dataIdx;
};

#endif
