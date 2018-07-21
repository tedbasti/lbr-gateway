/*
 * ioconfig.h
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#ifndef SRC_IOCONFIG_H_
#define SRC_IOCONFIG_H_

//#include <avr/io.h>


#define RX_DDR DDRD
#define RX_PIN PIND
#define RX_DATA_PIN PD2

#define TX_DDR DDRB
#define TX_PORT PORTB
#define TX_VCC_PIN PB4
#define TX_DATA_PIN PB5

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_TX_PIN PD4
#define LED_ERROR_PIN PD7




#endif /* SRC_IOCONFIG_H_ */
