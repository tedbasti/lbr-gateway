/*
 * import.h
 *
 *  Created on: Jul 12, 2018
 *      Author: ted
 */

#ifndef SRC_UTIL_CONFIGEXTERN_H_
#define SRC_UTIL_CONFIGEXTERN_H_

#define TRANSMIT_BUFFER_SIZE 100
#define RECEIVE_BUFFER_SIZE 1024

#include <stdint.h>
#include "../radio/databuffer.h"

namespace CONFIG {
	extern uint8_t senderId;
	extern uint8_t receiverId;
	extern uint8_t layerConfig;
	extern uint8_t payloadLen;
}

typedef bool (*callbackFunc)(DataBuffer<TRANSMIT_BUFFER_SIZE> &transmitBuffer);
namespace MAIN {
	extern callbackFunc onHandlingNeeded;
	extern callbackFunc sendData;
}

#endif /* SRC_UTIL_CONFIGEXTERN_H_ */