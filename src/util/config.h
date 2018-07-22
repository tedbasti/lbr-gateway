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
	uint8_t volatile senderId=0;
	uint8_t volatile receiverId=0;
	uint8_t volatile layerConfig=0;
	uint8_t volatile payloadLen=0;

}

#endif /* SRC_UTIL_CONFIG_H_ */
