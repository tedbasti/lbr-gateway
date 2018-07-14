#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

namespace Timer {

/* Overflow interrupt */
void setupTimer8bit();

/* Compare match interrupt */
void setupTimer16bit(uint16_t prescaler, uint16_t frequency);

}

#endif
