#ifndef TIMERCONF_H
#define TIMERCONF_H

#include <stdint.h>

#define FOSC_DEF 16000000	// Quartz oscillator frequency [Hz]
#define PRESCALER_DEF 256	// Prescaler
#define FOC_DEF 2000		// Timer Compare Interrupt frequency [Hz]
#define OCRNX_DEF uint16_t(FOSC_DEF/PRESCALER_DEF/FOC_DEF - 1)	// Output Compare Register Value


namespace TIMERCONF {

const uint16_t FOSC = (uint16_t) FOSC_DEF;
const uint16_t PRESCALER = (uint16_t) PRESCALER_DEF;
const uint16_t FOC = (uint16_t) FOC_DEF;
const uint16_t OCRNX = OCRNX_DEF;

}

#endif
