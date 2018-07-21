/*
 * io.cpp
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */


#include <stdint>
#include "io.h"
#include "ioconfig.h"


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

	bool rxRead() {
		return readPin(RX_PORT, RX_DATA_PIN);
	}

	void txVCCEnable() {
		setPin(TX_PORT, TX_ENABLE_PIN, 1);
	}

	void txVCCDisable() {
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
