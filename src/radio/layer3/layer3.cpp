
#include <string.h>
#include "layer3.h"
#include "../layer2/layer2.h"
#include "../../util/configExtern.h"
#include "../../usart/usart.h"


namespace MAIN {
	extern DataBuffer<TRANSMIT_BUFFER_SIZE> transmitBuffer;
}

namespace LAYER3 {

	volatile static bool timerActivated = false;
	volatile static uint16_t timerCounter = 0;

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
		return ((transmitbuffer.isEmpty() == false) && (timerActivated == false));
	}

	bool sendData(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitbuffer) {
		uint8_t packet[MAX_PAYLOAD_LEN];
		const DataSet * payload = transmitbuffer.peekFront();
		packet[0] = PACKET_CODE_DATA;
		memcpy(&packet[1], payload, CONFIG::payloadLen);
		LAYER2::transmitData(CONFIG::receiverId, packet, CONFIG::payloadLen + 1);
		timerActivated = true;
	}

	void receiveData(const uint8_t *data, uint8_t len) {
		switch(data[0]) {
			case PACKET_CODE_DATA:
				uint8_t packet[1];
				packet[0] = PACKET_CODE_ACK;
				LAYER2::transmitData(CONFIG::receiverId, packet, 1);
				USART::transmit(data+1, len-1);
				DEBUG_PRINT('D');
				break;

			case PACKET_CODE_ACK:
				if (!MAIN::transmitBuffer.isEmpty()) {
					MAIN::transmitBuffer.popFront();
				}
				DEBUG_PRINT('A');
				break;
			default:
				DEBUG_PRINT('X');
				break;
		}
	}
}
