#ifndef FRAME_H
#define FRAME_H

typedef uint8_t ClientID;

struct Frame {
	ClientID sender;
	ClientID receiver;
	uint8_t const *payload;
	uint8_t payloadLen;
};

#endif
