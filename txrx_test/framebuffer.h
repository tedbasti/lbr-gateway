#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "bitringbuffer.h"
#include "encoding.h"
#include "frame.h"

/* The Framebuffer is a BitRingBuffer that fully encodes
 * a frame (sender, receiver, payload) together with checksum
 * into the wire format, to be sent out on the data pin.
 */

template<BufferSize Size, typename ChecksumAlgo>
class FrameBuffer : public BitRingBuffer<Size> {
typedef BitRingBuffer<Size> Base;
public:
	FrameBuffer() {
	}
	
	bool hasSpaceForPayload(uint8_t payloadLen) const {
		return encodedFrameSize(payloadLen) <= this->remainingSpace();
	}
	
	void queueFrame(const Frame &frame) {
		assert(hasSpaceForPayload(frame.payloadLen));
		
		ChecksumAlgo ckSum;
		
		/* Send warmup bytes; these are apparently necessary
		 * for a reliable transmission */
		Encoding::pushByte(*this, 0xF0);
		
		Encoding::pushStartSeq(*this);
		
		Encoding::pushByte(*this, frame.sender);
		ckSum.addByte(frame.sender);
		
		Encoding::pushByte(*this, frame.receiver);
		ckSum.addByte(frame.receiver);
		
		Encoding::pushBytes(*this, frame.payload, frame.payloadLen);
		ckSum.addBytes(frame.payload, frame.payloadLen);
		
		Encoding::pushByte(*this, ckSum.getDigest());
		
		Encoding::pushEndSeq(*this);
	}
	
private:
	/* Returns size that a full ENCODED frame occupies on buffer */
	BufferSize encodedFrameSize(uint8_t payloadLen) const {
		/* Number bytes: senderID + receiverID + payload + crc
		 * Number bits: bytes * 16 (manchester) + 8*2 (start/stop seq, 8bit) */
		BufferSize frameBytes = (2 + payloadLen + 1);
		return Encoding::encodedByteSize(frameBytes) + Encoding::startEndSeqSize;
	}
};

#endif
