#ifndef TIMERCONF_H
#define TIMERCONF_H

#include <stdint.h>

namespace TimerConf {

#define FOSC 16000000							// Quartz oscillator frequency [Hz]
#define PRESCALER 256							// Prescaler
#define FOC 10									// Timer Compare Interrupt frequency [Hz]
#define OCRNX uint16_t(FOSC/PRESCALER/FOC - 1)	// Output Compare Register Value

}

#endif
