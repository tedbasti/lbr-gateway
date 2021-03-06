/*
 * import.h
 *
 *  Created on: Jul 12, 2018
 *      Author: ted
 */

#ifndef SRC_UTIL_CONFIGEXTERN_H_
#define SRC_UTIL_CONFIGEXTERN_H_

#define TRANSMIT_BUFFER_SIZE 100
#define RECEIVE_BUFFER_SIZE 2048

//#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(x) USART::transmitChar(x)
#else
#define DEBUG_PRINT(x)
#endif

#define MAX_PAYLOAD_LEN 5

#include <stdint.h>

namespace CONFIG {
	extern volatile uint8_t senderId;
	extern volatile uint8_t receiverId;
	extern volatile uint8_t layerConfig;
	extern volatile uint8_t payloadLen;
}

#endif /* SRC_UTIL_CONFIGEXTERN_H_ */
