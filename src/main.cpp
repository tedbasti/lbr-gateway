#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart.h"
#include "timer.h"
#include "timerconf.h"

#include "receiver.h"
#include "transmitter.h"

// timer0 (8bit) overflow interrupt
ISR (TIMER0_OVF_vect) {
}

// timer1 (16bit) compare match interrupt
ISR (TIMER1_COMPA_vect) {
	// These ifdefs are for compiling two individual binaries,
	// one for receiver and transmitter each (debugging only)
#ifdef TRANSMITTER
	Transmitter::onTimeSlice();
#elif defined(RECEIVER)
	Receiver::onTimeSlice();
#else
#error "Neither TRANSMITTER or RECEIVER defined"
#endif
}

int main (void)
{
	/* Ports D7, B5 (LED) as output */
	DDRD |= (1<<PD7);
	DDRB |= (1<<PB5);
	USART::init(Transmitter::usartCallback);

//	Timer::setupTimer8bit();
	Timer::setupTimer16bit(TimerConf::Prescale, TimerConf::PeriodLength);
	sei();

	while(1) {
		_delay_ms(100000);
	}
}
