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

// Reference:
// https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
void setupTimer16bit(uint8_t prescaler, uint16_t period) {
	assert(prescaler <= 0x7);
	
	OCR1A = period;

    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    // Set interrupt on compare match

    TCCR1B |= (prescaler << CS10);
    // set prescaler and start timer
}

}
