#include <avr/io.h>
#include <assert.h>

#include "timer.h"

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
void setupTimer16bit(uint16_t prescaler, uint16_t frequency) {
	bool prescalerCorrect = ((prescaler == 1)
			|| (prescaler == 8)
			|| (prescaler == 64)
			|| (prescaler == 256)
			|| (prescaler == 1024));

	// Assert that the prescaler value is set correctly
	assert(prescalerCorrect);

	// Assert that the specified frequency can be established with the specified prescaler.
	assert(1 <= (16000000/prescaler/frequency/2 - 1) <= 65535);

	// Reset TC1 Control Register A
	TCCR1A = 0x00;

	// Initialize TC1 Counter Value Low byte and High byte
	TCNT1L = 0x00;
	TCNT1H = 0x00;

	// Set Output Compare Register for specified frequency
	OCR1A = (uint16_t) 16000000/prescaler/frequency/2 - 1;

	// Waveform Generation Mode 4 (CTC)
	TCCR1B |= (1 << WGM12);

	// Set prescaler
	switch(prescaler) {
		case 1:
			TCCR1B |= (1 << CS10);
			break;
		case 8:
			TCCR1B |= (1 << CS11);
			break;
		case 64:
			TCCR1B |= (1 << CS11) | (1 << CS10);
			break;
		case 256:
			TCCR1B |= (1 << CS12);
			break;
		case 1024:
			TCCR1B |= (1 << CS12) | (1 << CS10);
	}

	// Enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}

}
