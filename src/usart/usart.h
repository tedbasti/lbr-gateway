
/*
 * usart_new.h
 *
 *  Created on: Jun 23, 2018
 *      Author: ramon-pio
 */

/* From console:
 *
 * Setup USART:
 * stty -F /dev/<arduino> cs8 9600 ignbrk -brkint -icrnl -imaxbel \
 * -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl \
 * -echoke noflsh -ixon -crtscts
 *
 * Reading:
 * cat < /dev/<arduino>
 *
 * Write char 'c':
 * echo -n "c" > /dev/<arduino>
 *
 */

#ifndef SRC_USART_USART_H_
#define SRC_USART_USART_H_


#include <stdint.h>


/*
 * Provides functions for transmitting and receiving data via USART.
 * When USART gets initialized, the USART baud rate will be set and
 * transmitting and receiving via USART will be enabled.
 */
namespace USART {
	/*
	 * Sets the USART baud rate, enables transmitting and receiving via
	 * USART.
	 *
	 */
	void init();

	/*
	 * Transmits data via USART.
	 *
	 * PARAMETERS
	 *
	 * 	data: unsigned char
	 */
	void transmitChar(unsigned char data);

	void transmit(const uint8_t *data, uint8_t len);

	void flush();
}


#endif /* SRC_USART_USART_H_ */
