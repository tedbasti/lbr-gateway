/*
 * io.cpp
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */

#define RX_DDR DDRD
#define RX_PORT PIND
#define RX_DATA_PIN PD2

#define TX_DDR DDRB
#define TX_PORT PORTB
#define TX_ENABLE_PIN PB4
#define TX_DATA_PIN PB5

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_TX_PIN PD4
#define LED_ERROR_PIN PD7


#include <stdint>
#include "io.h"


namespace IO {
	void static setPin(uint8_t port, uint8_t pin, uint8_t value) {
		if(value != 0) {
			port |= (1 << pin);
		}
		else {
			port &= ~(1 << pin);
		}
	}

	bool static readPin(uint8_t port, uint8_t pin) {
		return (PIND & (1 << pin));
	}

	void init() {
		// Initialize RX data direction register.
		RX_DDR &= (0 << RX_DATA_PIN);

		// Initialize TX data direction register.
		TX_DDR |= (1 << TX_ENABLE_PIN) | (1 << TX_DATA_PIN);

		// Initialize LED data direction register.
		LED_DDR |= (1 << LED_TX_PIN) | (1 << LED_ERROR_PIN);
	}

	bool rxRead(uint8_t port, uitn8_t pin) {
		return readPin(RX_PORT, RX_DATA_PIN);
	}

	void txEnable() {
		setPin(TX_PORT, TX_ENABLE_PIN, 1);
	}

	void txDisable() {
		setPin(TX_PORT, TX_ENABLE_PIN, 0);
	}

	void txWrite(bool value) {
		setPin(TX_PORT, TX_DATA_PIN, value);
	}

	void ledTXWrite(bool value) {
		setPin(LED_PORT, LED_TX_PIN, value);
	}

	void ledErrorWrite(bool value) {
		setPin(LED_PORT, LED_ERROR_PIN, value);
	}
}
