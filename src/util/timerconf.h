#ifndef TIMERCONF_H
#define TIMERCONF_H

#include <stdint.h>

namespace TIMERCONF {

#define FOSC_DEF 16000000							// Quartz oscillator frequency [Hz]
#define PRESCALER_DEF 256							// Prescaler
#define FOC_DEF 2000								// Timer Compare Interrupt frequency [Hz]
#define OCRNX_DEF uint16_t(FOSC/PRESCALER/FOC - 1)	// Output Compare Register Value

const uint16_t PRESCALER = (uint16_t) PRESCALER_DEF;
const uint16_t FOC = (uint16_t) FOC_DEF;
const uint16_t OCRNX = OCRNX_DEF;

}

#endif
