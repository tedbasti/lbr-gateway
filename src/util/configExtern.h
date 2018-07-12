/*
 * import.h
 *
 *  Created on: Jul 12, 2018
 *      Author: ted
 */

#ifndef SRC_UTIL_CONFIGEXTERN_H_
#define SRC_UTIL_CONFIGEXTERN_H_

#include <stdint.h>

namespace CONFIG {
	extern uint8_t senderId;
	extern uint8_t receiverId;
	extern uint8_t layerConfig;
	extern uint8_t payloadLen;
}

#endif /* SRC_UTIL_CONFIGEXTERN_H_ */
