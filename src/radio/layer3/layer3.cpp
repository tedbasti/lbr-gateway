
#include <string.h>
#include "layer3.h"
#include "../layer2/layer2.h"
#include "../../util/configExtern.h"
#include "../../usart/usart.h"
#include <util/delay.h>


namespace MAIN {
	extern DataBuffer<TRANSMIT_BUFFER_SIZE> transmitBuffer;
}

namespace LAYER3 {

	volatile static bool timerActivated = false;
	volatile static uint16_t timerCounter = 0;
	static uint8_t packetNumber = 0;
	static bool firstPacketReceived = false;

	void onTime() {
		if(timerActivated) {
			timerCounter += 1;
			if(timerCounter == TIMEOUT) {
				timerCounter = 0;
				timerActivated = false;
			}
		}
	}

	bool onHandlingNeeded(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitbuffer) {
		return (LAYER2::onHandlingNeeded(transmitbuffer) && (timerActivated == false));
	}

	bool sendData(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitbuffer) {
		// Create packet
		uint8_t packet[MAX_PAYLOAD_LEN + 1];
		const uint8_t HEADER = (packetNumber << 4) | ((uint8_t) PACKET_CODE_DATA);
		const DataSet * PAYLOAD = transmitbuffer.peekFront();
		packet[0] = HEADER;
		memcpy(&packet[1], PAYLOAD, CONFIG::payloadLen);

		// Transmit packet
		LAYER2::transmitData(CONFIG::receiverId, packet, CONFIG::payloadLen + 1);

		// Activate timer
		timerActivated = true;
	}

	void receiveData(const uint8_t *data, uint8_t len) {
		// Reading header
		const uint8_t PACKET_NUMBER = (data[0] >> 4);
		const uint8_t PACKET_CODE = 0x0F & data[0];	// 0x0F = 00001111

		switch(PACKET_CODE) {
			case PACKET_CODE_DATA:
				// Create and send ACK
				uint8_t packet[1];
				packet[0] = (PACKET_NUMBER << 4) | ((uint8_t) PACKET_CODE_ACK);
				_delay_ms(DELAY_ACK_PACKET_MS);
				LAYER2::transmitData(CONFIG::receiverId, packet, 1);

				// If this is the first data packet which the slave is receiving.
				if(firstPacketReceived == false) {
					packetNumber = PACKET_NUMBER;
					firstPacketReceived = true;
					USART::transmit(data+1, len-1);
					DEBUG_PRINT('D');
				}
				else {
					// If data packet is not a duplicate
					if(PACKET_NUMBER != packetNumber) {
						USART::transmit(data+1, len-1);
						packetNumber = PACKET_NUMBER;
						DEBUG_PRINT('D');
					}
				}

				break;

			case PACKET_CODE_ACK:
				if(PACKET_NUMBER == packetNumber) {
					packetNumber = (packetNumber + 1)%2;
					if(!MAIN::transmitBuffer.isEmpty()) {
						MAIN::transmitBuffer.popFront();
						DEBUG_PRINT('A');
					}
				}
				break;
			default:
				DEBUG_PRINT('X');
				break;
		}
	}
}
