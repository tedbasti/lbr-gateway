/*
 * USART-Transmitter:
 * Das ist ein einfacher USART Transmitter.
 * Dieser sendet im Sekundentakt zuerst
 * 'a' und anschliessend 'b' ueber die
 * USART Schnittstelle.
 * Gesendet wird mit 9600 Baud.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "usart.h"

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

static USART::CallbackFun _receiveCb;

ISR(USART_RX_vect) {
	unsigned char x = UDR0;
	_receiveCb(x);
}

namespace USART {
static void dummyCallback(unsigned char) {}

/**
 * Fast 1 zu 1 aus dem Datenblatt entmommen.
 */
static void initRegisters(unsigned int ubrr) {
	/*Set baud rate */
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
	sei();
}

void init(CallbackFun cb) {
	initRegisters(MYUBRR);
	_receiveCb = cb ? cb : dummyCallback;
}

void transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1 << UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void transmit(char const *data) {
	for (char const *p = data; *p; ++p) {
		transmit(*p);
	}
}

void transmit(uint8_t const *data, uint8_t len) {
	for (uint8_t i = 0; i < len; ++i) {
		transmit(data[i]);
	}
}

}
