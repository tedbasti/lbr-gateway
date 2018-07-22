#ifndef TIMERCONF_H
#define TIMERCONF_H

#include <stdint.h>
#include "../radio/layer1/layer1.h"

// Quartz oscillator frequency [Hz]
#define FOSC_DEF 16000000

// Prescaler
#define PRESCALER_DEF 8

/**
 * Layer 1 sampling rate [Hz]
 * TX radio module data rate: 8000 bps
 * RX radio module data rate: 4800 bps
 */
#define WANTED_FREQ 1000
#define FOC_DEF 10000

// Layer1 Timer output compare register value
#define OCRNX_DEF uint16_t(FOSC_DEF/PRESCALER_DEF/FOC_DEF - 1)


namespace TIMERCONF {

const uint16_t FOSC = (uint16_t) FOSC_DEF;
const uint16_t PRESCALER = (uint16_t) PRESCALER_DEF;
const uint16_t FOC = (uint16_t) FOC_DEF;
const uint16_t OCRNX = OCRNX_DEF;

}

#endif
