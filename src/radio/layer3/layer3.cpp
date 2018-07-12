
#include <string.h>
#include "layer3.h"
#include "../layer2/layer2.h"
#include "../../util/configExtern.h"

namespace LAYER3 {

	static bool timerActivated = false;
	static uint8_t timerCounter = 0;

	void onTime() {
		if(timerActivated) {
			timerCounter += 1;
		}
	}

	bool onHandlingNeeded(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitbuffer) {
		switch(timerActivated) {
			case true:
				if(timerCounter == 255) {
					sendData(transmitbuffer);
				}
				break;

			case false:
				if(transmitbuffer.isEmpty() == false) {
					sendData(transmitbuffer);
					timerActivated = false;
					timerCounter = 0;
				}
				break;
		}
	}

	bool sendData(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitbuffer) {
		uint8_t packet[CONFIG::payloadLen + 1];
		const DataSet * payload = transmitbuffer.peekFront();
		packet[0] = CONFIG::senderId;
		memcpy(&packet[1], payload, CONFIG::payloadLen);
		LAYER2::transmitData(CONFIG::receiverId, packet, CONFIG::payloadLen);
	}
}
