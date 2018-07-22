#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart/usart.h"
#include "util/timer.h"
#include "util/timerconf.h"
#include "util/configExtern.h"
#include "util/io.h"

#include "radio/layer2/layer2.h"
#include "radio/layer1/layer1.h"
#include "radio/layer3/layer3.h"
#include "radio/bitringbuffer.h"
#include "radio/databuffer.h"

// timer0 (8bit) overflow interrupt
ISR (TIMER0_OVF_vect) {
}

// timer1 (16bit) compare match interrupt
ISR (TIMER1_COMPA_vect) {
	LAYER1::onTimeReceive();
	if (LAYER1::onTimeTransmit()) {
		LAYER3::onTime();
	}
}

namespace MAIN {
/*
 * Within that receiveBuffer, layer1. will put all
 * the received bits, so that the main loop, could read them!
 */
	typedef bool (*callbackFunc)(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer);
	BitRingBuffer<RECEIVE_BUFFER_SIZE> receiveBuffer;
	DataBuffer<TRANSMIT_BUFFER_SIZE> transmitBuffer;
//	typedef bool (*callbackFunc)(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer);

	callbackFunc volatile onHandlingNeeded = LAYER2::onHandlingNeeded;
	callbackFunc volatile sendData = LAYER2::sendData;
}

int main (void) {
	// Disable interrupts globally.
	cli();

	// Initializing I/O setup.
	IO::init();

	// Initializing 16bit timer/counter.
	Timer::setupTimer16bit();

	// Initializing USART.
	USART::init();

	// Enable interrupts globally.
	sei();

	while(1) {
		//Receive from the receiveBuffer and put it on layer2
		if(MAIN::receiveBuffer.isEmpty() == false) {
			bool bitValue = MAIN::receiveBuffer.popBit();
			LAYER2::receiveBit(bitValue);
		}
		if (MAIN::onHandlingNeeded(MAIN::transmitBuffer)) {
			MAIN::sendData(MAIN::transmitBuffer);
		}
	}
}
