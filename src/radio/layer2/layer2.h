/*
 * layer2.h
 *
 * This layer is there to encode stuff and so on.
 * Look at the description of the functions
 * - rcvBit(bool bit)
 * - sendDate(uint8_t rcv, char* data, uint8_t len)
 * fore more information.
 *
 * This layer communicates only with the
 * layer1 or with layer3/usart interface!
 */

#ifndef SRC_RADIO_LAYER2_LAYER2_H_
#define SRC_RADIO_LAYER2_LAYER2_H_

#define PAUSE_AFTER_RECEIVE_PACKAGE 100

#define MAX_BUFFER_SPACE_NEEDED_FOR_PACKAGE 256
#include <stdint.h>
#include "../databuffer.h"
#include "../../util/configExtern.h"

namespace LAYER2 {
	/*
	 * This function is called by the lower layer (layer 1)
	 * and rcv 1 bit.
	 * This function is responsible for:
	 * - Find out where the frame starts and ends (start and end sequence)
	 * - Build a frame upon the receiving bits
	 * - Decoding
	 * - Check if the checksum is right
	 * - Check if the receiver is the right one (otherwise drop)
	 */
	void receiveBit(bool bit);

	/**
	 * This function is called by the higher layer (layer3
	 * or the usart receiver).
	 * This function is responsible for:
	 * - Encoding
	 * - Checksum generation
	 * - Build a frame with transmitterId and receiverId
	 * (- Send start and end sequence)
	 */
	void transmitData(uint8_t rcv, uint8_t* data, uint8_t len);

	/*
	 * Need something to be done within the main loop?
	 */
	bool onHandlingNeeded(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer);

	/*
	 * Sends the data, from the transmitbuffer
	 */
	bool sendData(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer);

	bool sendBufferEnoughSpace();

	void onTime();

	bool isInPause();
}


#endif /* SRC_RADIO_LAYER2_LAYER2_H_ */
