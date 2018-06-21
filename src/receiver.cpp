
#include <stdint.h>
#include <avr/io.h>

#include "receiver.h"
#include "framereceivebuffer.h"
#include "checksum.h"
#include "util.h"

#include "usart.h"

namespace Receiver {

static FrameReceiveBuffer<32, XorChecksum8> frcvBuf;

static void onFrameCompleted() {
	if (frcvBuf.hasValidFrame()) {
			Frame frame;
			frcvBuf.getFrame(frame);
			USART::transmit("\nSender: ");
			USART::transmit(frame.sender);
			USART::transmit("\nReceiver: ");
			USART::transmit(frame.receiver);
			USART::transmit("\nPayload: ");
			USART::transmit(frame.payload, frame.payloadLen);
			USART::transmit('\n');
		} else {
			USART::transmit("\n[Invalid packet]\n");
		}
}

void onTimeSlice() {
	uint8_t dataBit = DATA_IN ? 1 : 0;
	
	/* Visualize DATA_IN on LED */
	SET_P(PORTB, PB5, dataBit);

	/* This is true when a valid start sequence was received
	 * and encoded data bits are being recorded */
	static bool rcvData = false;
	
	/* Initialize this to something that can never be shifted
	 * into a valid sequence with non-sequence bits */
	static uint8_t seq = 0x55;

	seq = (seq << 1) | dataBit;

	if (rcvData) {
		/* If an encoding (manchester) error was generated,
		 * either the transmission was faulty, or the
		 * end sequence was received, which deliberately violates
		 * the encoding */
		if (!frcvBuf.pushEncodedBit(dataBit)) {
			if (Encoding::isEndSeq(seq)) {
				onFrameCompleted();
				frcvBuf.clear();
			}
			
			USART::transmit('!');
			rcvData = false;
		}
	}
	
	/* Even when a start sequence has been received previously,
	 * if a new start sequence comes in, the state has to be
	 * reset and data recording restarted, because the 1st start seq
	 * might have been noise, while the 2nd one was from a
	 * valid transmission. */
	if (Encoding::isStartSeq(seq)) {
		if (rcvData) {
			/* Received two start sequences without an end sequence.. */
			USART::transmit('?');
		}
		
		rcvData = true;
		frcvBuf.clear();
	}
}

}
