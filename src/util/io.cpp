/*
 * io.cpp
 *
 *  Created on: Jul 21, 2018
 *      Author: rambonator
 */


#include <stdint.h>
#include <avr/io.h>
#include "io.h"
#include "ioconfig.h"

#define SET_P(port, pin, val) (val) ? (port |= (1<<pin)) : (port &= ~(1<<pin))
#define READ_P(port, pin) (port & (1 << pin))


namespace IO {

	void init() {
		// Initialize RX data direction register.
		RX_DDR &= (0 << RX_DATA_PIN);

		// Initialize TX data direction register.
		TX_DDR |= (1 << TX_VCC_PIN) | (1 << TX_DATA_PIN);

		// Initialize LED data direction register.
		LED_DDR |= (1 << LED_TX_PIN) | (1 << LED_ERROR_PIN);
	}

	bool rxRead() {
		return READ_P(RX_PIN, RX_DATA_PIN);
	}

	void txVCCEnable() {
		SET_P(TX_PORT, TX_VCC_PIN, 1);
	}

	void txVCCDisable() {
		SET_P(TX_PORT, TX_VCC_PIN, 0);
	}

	void txWrite(bool value) {
		SET_P(TX_PORT, TX_DATA_PIN, value);
	}

	void ledTXWrite(bool value) {
		SET_P(LED_PORT, LED_TX_PIN, value);
	}

	void ledErrorWrite(bool value) {
		SET_P(LED_PORT, LED_ERROR_PIN, value);
	}
}
