#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

namespace Timer {

/* Overflow interrupt */
void setupTimer8bit();

/* Compare match interrupt */
void setupTimer16bit(uint8_t prescaler, uint16_t period);

}

#endif
