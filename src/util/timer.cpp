#include <avr/io.h>
#include <assert.h>

#include "timer.h"
#include "timerconf.h"

namespace Timer {

	void setupTimer8bit() {
		//Aktiviere timer 0 (Stromsparmodus für timer 0 deaktivieren...)
		PRR &= ~(1<<PRTIM0);
		//Prescaler genau auf 1 setzen. Jeder Takt.
		TCCR0B |= 0x1;
		//Aktiviere overflow interrupt für timer 0
		TIMSK0 |= (1<<TOIE0);
	}

	/**
 	 * This function initializes Timer/Counter 1 in Waveform generation mode 4
 	 * so that the timer compare interrupt will occure with the specified
 	 * frequency.
 	 */
	void setupTimer16bit() {
		// Assert that the specified frequency can be established with the specified prescaler.
		assert((1 <= TIMERCONF::OCRNX) && (TIMERCONF::OCRNX <= 65535));

		// Set Output Compare Register for specified frequency
		OCR1A = OCRNX;

		// Waveform Generation Mode 4 (CTC)
		TCCR1B |= (1 << WGM12);

		// Enable Timer Compare Interrupt
		TIMSK1 |= (1 << OCIE1A);

		// Set prescaler
		switch(TIMERCONF::PRESCALER) {
			case 0:
				TCCR1B &= 0b11111000;
				break;
			case 1:
				TCCR1B |= (1 << CS10);
				break;
			case 8:
				TCCR1B |= (1 << CS11);
				break;
			case 256:
				TCCR1B |= (1 << CS12);
				break;
			case 1024:
				TCCR1B |= (1 << CS12) | (1 << CS10);
				break;
		}
	}

}
