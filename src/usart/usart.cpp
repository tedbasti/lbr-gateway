
#include "usart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define FOSC 16000000			// System clock Speed
#define BAUD 57600				// Baud rate
#define UBRR FOSC/(16*BAUD)-1


ISR(USART_RX_vect) {
	unsigned char data = UDR0;
	_cbFunc(data);
}


static USART::callbackFunc _cbFunc;

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
