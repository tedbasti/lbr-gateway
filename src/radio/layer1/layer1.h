/*
 * layer1.h
 *
 *  Created on: Jun 23, 2018
 *      Author: ted
 */

#ifndef SRC_RADIO_LAYER1_LAYER1_H_
#define SRC_RADIO_LAYER1_LAYER1_H_

#include <avr/io.h>
#include <stdint.h>

#define MAX_OFFSET_COUNTER 40

namespace LAYER1 {

/* Port and pin connected to transmitter module DATA */
#define DATA_OUT_PORT PORTD
#define DATA_OUT_PIN PD7
#define DATA_IN (PIND & (1<<PD4))

void init();

bool sendBit(bool bit);

bool onTimeTransmit();

void onTimeReceive();

uint16_t getTXBufferSpace();

}
#endif /* SRC_RADIO_LAYER1_LAYER1_H_ */
