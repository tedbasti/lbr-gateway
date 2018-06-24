#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

namespace LAYER2 {
typedef uint8_t ClientID;

struct Frame {
	uint8_t payloadLen;
	ClientID receiver;
	ClientID sender;
	uint8_t payload[5];
};

}
#endif
