
#include "usart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define FOSC 16000000				// System clock Speed
#define BAUD 57600					// Baud rate
#define UBRR int(FOSC/(16*BAUD)-0.5)

static USART::callbackFunc _cbFunc;

/*
 * This function has two states:
 * - Initialisation State (0)
 * - Receive State (Direct connection) (1)
 * - Receive State (Multi Connection) (2)
 *
 * Initialisation State:
 * Within the Initialisation State the
 * the logic expects special data:
 * Byte[0]: 0xFF: Just for check that
 * 			it's really a initalisation
 * Byte[1]: TransmitterID: The TransmitterID
 * Byte[2]: ReceiverID: The Receiver
 * Byte[3]: Bytes to send:
 * 			From 0x01 to 0x05
 * Byte[4]: The Protocol Type:
 * 		Direct Connection(0x01)
 * 		or Multi Connection(0x02).
 *
 * 		Direct Connection:
 * 			Receiver is fix.
 *
 * 		Multi Connection:
 * 			Receiver will be the first byte within the future
 */

#define STATE_INITIAL 0
#define STATE_INITIAL_SIZE 5
#define STATE_DIRECT_CONNECTION 1
#define STATE_DIRECT_CONNECTION_SIZE 5
#define STATE_MULTI_CONNECTION 2
#define STATE_MULTI_CONNECTION_SIZE 5

ISR(USART_RX_vect) {
	//Read the Data
	unsigned char data = UDR0;
	/*
	 * - buffer is there to hold more than one char/uint8_t
	 * - bufferPos holds the current pos, where to write
	 * - state is the current state.
	 * For state descriptions look above.
	 */
	uint8_t buffer[6];
	uint8_t bufferPos = 0;
	uint8_t state = STATE_INITIAL;
	switch (state) {
	case STATE_INITIAL:
		//TODO: Set some configuration information
	break;
	case STATE_DIRECT_CONNECTION:
		//TODO: Receive data and send to layer2/layer3
	}

	_cbFunc(data);
}


namespace USART {

	void init(callbackFunc function) {
		// Set baud rate
		UBRR0H = (unsigned char) (UBRR >> 8);
		UBRR0L = (unsigned char) UBRR;

		// Enable receiver and transmitter
		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

		// Set frame format: 8data, 2stop bit
		UCSR0C = (1 << USBS0) | (3 << UCSZ00);

		_cbFunc = function;

		sei();
	}

	void transmit(unsigned char data) {
		// Wait for empty transmit buffer
		while (!( UCSR0A & (1 << UDRE0)));

		// Push data into transmit buffer
		UDR0 = data;
	}

	void transmit(const char *data) {
		for(const char *p = data; *p; ++p) {
			transmit(*p);
		}
	}

	void transmit(const uint8_t *data, uint8_t len) {
		for(uint8_t i = 0; i < len; i++) {
			transmit(data[i]);
		}
	}

	void flush() {
		unsigned char dummy;
		while (UCSR0A & (1 << RXC0)) {
			dummy = UDR0;
		}
	}

}
