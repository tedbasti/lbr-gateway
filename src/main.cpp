#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart/usart.h"
#include "util/timer.h"
#include "util/timerconf.h"

#include "radio/layer2/layer2.h"
#include "radio/layer1/layer1.h"

// timer0 (8bit) overflow interrupt
ISR (TIMER0_OVF_vect) {
}

// timer1 (16bit) compare match interrupt
ISR (TIMER1_COMPA_vect) {
	LAYER1::onTimeTransmit();
	LAYER1::onTimeReceive();
}

int main (void) {
	/* Ports D7 as output */
	DDRD |= (1<<PD7);
	USART::init(LAYER2::transmitData);

	Timer::setupTimer16bit(TimerConf::Prescale, TimerConf::PeriodLength);
	sei();

	while(1) {
		//Do nothing, coz everything is currently within the
	}
}
