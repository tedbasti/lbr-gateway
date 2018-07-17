/*
 * config.h
 *
 *  Created on: Jun 27, 2018
 *      Author: ted
 */

#ifndef SRC_UTIL_CONFIG_H_
#define SRC_UTIL_CONFIG_H_
#include <stdint.h>

namespace CONFIG {
	uint8_t volatile senderId;
	uint8_t volatile receiverId;
	uint8_t volatile layerConfig;
	uint8_t volatile payloadLen;

}

#endif /* SRC_UTIL_CONFIG_H_ */
