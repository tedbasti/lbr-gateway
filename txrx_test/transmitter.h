#ifndef TRANSMITTER_H
#define TRANSMITTER_H

namespace Transmitter {

/* Port and pin connected to transmitter module DATA */
#define DATA_OUT_PORT PORTD
#define DATA_OUT_PIN PD7

/* Send function, checks ringbuffer for outstanding
 * data and sends it on output pin */
void onTimeSlice();

/* This callback queues a frame for each char
 * sent via USART, for debugging purposes */
void usartCallback(unsigned char c);

}

#endif
