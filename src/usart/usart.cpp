#include "usart.h"
#include "../util/config.h"
#include "../radio/databuffer.h"
#include "../util/configExtern.h"
#include "../radio/layer3/layer3.h"
#include "../radio/layer2/layer2.h"

#include <avr/io.h>
#include <avr/interrupt.h>


#define FOSC 16000000				// System clock Speed
#define BAUD 1200					// Baud rate
#define UBRR int(FOSC/16/BAUD-0.5)

namespace MAIN {
	typedef bool (*callbackFunc)(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer);
	extern DataBuffer<TRANSMIT_BUFFER_SIZE> transmitBuffer;
	extern callbackFunc volatile onHandlingNeeded;
	extern callbackFunc volatile sendData;
}

namespace LAYER2 {
	typedef void (*nextLayerFunction)(const uint8_t *data, uint8_t len);
	extern nextLayerFunction volatile higherLayer;
}

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
#define LAYER2_CODE 2
#define LAYER3_CODE 3
#define STATE_INITIAL_SIZE 5
#define STATE_DIRECT_CONNECTION 1

/*
 * Initialize "protocol": (size 5)
 * ==================================
 * [SenderId]: The id used when sending frames
 * [ReceiverId]: The id used when receiving frames
 * 		(Frames with another receivers will be dropped)
 * [Layer2/Layer3]: Layer3 is layer2 with acknowledgement
 * 					packages
 * [LengthOfPayload]: between 1 and 5 bytes will be for data
 * [Protocol] DirectConnection of Multi/Connection.
 * At the moment not used and only there for future reasons!
 */

ISR(USART_RX_vect) {
	//Read the Data
	uint8_t data = UDR0;
	/*
	 * - buffer is there to hold more than one char/uint8_t
	 * - bufferPos holds the current pos, where to write
	 * - state is the current state.
	 * For state descriptions look above.
	 */
	static uint8_t buffer[6];
	static uint8_t bufferPos = 0;
	static uint8_t state = STATE_INITIAL;

	buffer[bufferPos++] = data;

	switch (state) {
	case STATE_INITIAL:
		if (bufferPos == STATE_INITIAL_SIZE) {
			bufferPos=0;
			CONFIG::senderId = buffer[0];
			CONFIG::receiverId = buffer[1];
			CONFIG::layerConfig = buffer[2];
			CONFIG::payloadLen = buffer[3];
			if ((CONFIG::layerConfig == LAYER2_CODE || CONFIG::layerConfig == LAYER3_CODE) &&
					CONFIG::payloadLen > 0 && CONFIG::payloadLen < 5) {
				//At the moment fix at STATE_DIRECT_CONNECTION
				state = STATE_DIRECT_CONNECTION;
				if (CONFIG::layerConfig == LAYER3_CODE) {
					MAIN::onHandlingNeeded = LAYER3::onHandlingNeeded;
					MAIN::sendData = LAYER3::sendData;
					LAYER2::higherLayer = LAYER3::receiveData;
				}
			}
		}
	break;
	case STATE_DIRECT_CONNECTION:
		if (bufferPos == CONFIG::payloadLen) {
			struct DataSet dataSet;
			memcpy(dataSet.payload, buffer, CONFIG::payloadLen);
			MAIN::transmitBuffer.pushBack(dataSet);
			bufferPos=0;
		}
		break;
	}
}


namespace USART {

	void init() {
		// Set baud rate
		UBRR0H = (unsigned char) (UBRR >> 8);
		UBRR0L = (unsigned char) UBRR;

		// Enable receiver and transmitter
		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

		// Set frame format: 8data, 2stop bit
		UCSR0C = (1 << USBS0) | (3 << UCSZ00);
	}

	void transmitChar(unsigned char data) {
		// Wait for empty transmit buffer
		while (!( UCSR0A & (1 << UDRE0)));

		// Push data into transmit buffer
		UDR0 = data;
	}

	void transmit(const uint8_t *data, uint8_t len) {
		for(uint8_t i = 0; i < len; i++) {
			transmitChar(data[i]);
		}
	}

	void flush() {
		unsigned char dummy;
		while (UCSR0A & (1 << RXC0)) {
			dummy = UDR0;
		}
	}

}
