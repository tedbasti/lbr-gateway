/*
 * layer3.h
 *
 *  Created on: Jul 12, 2018
 *      Author: rambonator
 */

#ifndef SRC_RADIO_LAYER3_LAYER3_H_
#define SRC_RADIO_LAYER3_LAYER3_H_

#define PACKET_CODE_DATA 0
#define PACKET_CODE_ACK 1

#define TIMEOUT (uint16_t) 400
#define TIMEOUT_MAX_RESEND 3

#define DELAY_ACK_PACKET_MS 0

#include <stdint.h>
#include "../databuffer.h"
#include "../../util/configExtern.h"


namespace LAYER3 {

	/**
	 * This function handles the layer 3 timing in relation to the layer 1 base frequency.
	 * It needs to be called on every overflow/compare interrupt of the timer/counter unit
	 * which sets the layer 1 base frequency.
	 */
	void onTime();

	/**
	 * This function examinates if a certain  dataset needs to be sended.
	 */
	bool onHandlingNeeded(DataBuffer<TRANSMIT_BUFFER_SIZE> &databuffer);

	/**
	 * This function creates and sends packets from the information in the DataBuffer object
	 * which is passed in the function argument.
	 */
	bool sendData(DataBuffer<TRANSMIT_BUFFER_SIZE> &databuffer);

	/**
	 * This function should be called from layer 2 and handles the packet passed in the
	 * function argument.
	 */
	void receiveData(const uint8_t *data, uint8_t len);
}




#endif /* SRC_RADIO_LAYER3_LAYER3_H_ */
