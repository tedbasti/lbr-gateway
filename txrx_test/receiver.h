#ifndef RECEIVER_H
#define RECEIVER_H

#include "transmitter.h"

//#define DATA_IN (PORTB & (1<<PB5))
#define DATA_IN (PIND & (1<<PD4))

namespace Receiver {

/* Handler function for receiver, to be called
 * on every timer interrupt */
void onTimeSlice();

}

#endif
