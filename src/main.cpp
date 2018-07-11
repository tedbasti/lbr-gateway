#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart/usart.h"
#include "util/timer.h"
#include "util/timerconf.h"

#include "radio/layer2/layer2.h"
#include "radio/layer1/layer1.h"
#include "radio/bitringbuffer.h"

// timer0 (8bit) overflow interrupt
ISR (TIMER0_OVF_vect) {
}

// timer1 (16bit) compare match interrupt
ISR (TIMER1_COMPA_vect) {
	LAYER1::onTimeTransmit();
	LAYER1::onTimeReceive();
}

namespace MAIN {
/*
 * Within that receiveBuffer, layer1. will put all
 * the received bits, so that the main loop, could read them!
 */
	BitRingBuffer<1024> receiveBuffer;
}

int main (void) {
	/* Ports D7 as output */
	DDRD |= (1<<PD7);
	USART::init(LAYER2::transmitData);

	Timer::setupTimer16bit(TimerConf::Prescale, TimerConf::PeriodLength);
	sei();

	while(1) {
		//Receive from the receiveBuffer and put it on layer2
		if(MAIN::receiveBuffer.isEmpty() == false) {
			bool bitValue = MAIN::receiveBuffer.popBit();
			LAYER2::receiveBit(bitValue);
		}
	}
}
