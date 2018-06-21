#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <assert.h>

#include "transmitter.h"
#include "frame.h"
#include "framebuffer.h"
#include "checksum.h"
#include "util.h"

#include "usart.h"

namespace Transmitter {

bool fakeDataIn = false;
static FrameBuffer<1024, XorChecksum8> ringBuf;

/* This function is called when the transmitter is 'idle',
 * meaning the outgoing ringbuffer has no bits to be sent.
 * Currently it queues a new 'ping' frame after a certain
 * amount of time */
static void onIdle() {
	static uint16_t idleCounter = 0;
	
	if (idleCounter++ < 0x200) {
		return;
	}
	
	idleCounter = 0;
	static uint8_t pingCount = 0;
	static uint8_t payload[] = "Ping: 00";
	
	Util::byteToHex(pingCount, (char*) payload+6);
	pingCount += 1;
	
	Frame frame = {
		'A', 'B',
		payload, sizeof(payload)
	};
	
	USART::transmit('P');
	
	/* Have to disable interrupts here so the timer isn't
	 * potentially triggered again; if that happened,
	 * we might start sending bits of a frame that is not
	 * fully queued up yet */
	cli();
	ringBuf.queueFrame(frame);
	
	/* Reset timer, just to be sure (probably not necessary) */
	TCNT1 = 0x00;
	sei();
}

void onTimeSlice() {
	if (ringBuf.isEmpty()) {
		onIdle();
		return;
	}
	
	bool outBit = ringBuf.popBit();
	SET_P(DATA_OUT_PORT, DATA_OUT_PIN, outBit);
}

void usartCallback(unsigned char c) {
	cli();
	uint8_t payload[] = "Hi USART";
	Frame frame = {
		c, 'X',
		payload, sizeof(payload)
	};
	ringBuf.queueFrame(frame);
	sei();
}

}
