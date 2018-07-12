
#include <string.h>
#include "layer3.h"
#include "../layer2/layer2.h"
#include "../../util/configExtern.h"

namespace LAYER3 {

	volatile static bool timerActivated = false;
	volatile static uint16_t timerCounter = 0;

	void onTime() {
		if(timerActivated) {
			timerCounter += 1;
			if(timerCounter == TIMEOUT) {
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
		timerCounter = 0;
		timerActivated = true;
	}

	void receiveData(const uint8_t *data, uint8_t len) {

	}
}
