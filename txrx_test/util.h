#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

/* macro for setting bit 'pin' on 'port' to 'val' */
#define SET_P(port, pin, val) (val) ? (port |= (1<<pin)) : (port &= ~(1<<pin))

namespace Util {

/* ASCII conversion functions */
static inline char nibbleToHex(uint8_t nib) {
	nib &= 0xF;
	if (nib < 0xA) {
		return '0' + nib;
	} else {
		return 'A' + (nib - 0xA);
	}
}

static inline void byteToHex(uint8_t byte, char *out) {
	out[0] = nibbleToHex((byte >> 4) & 0xF);
	out[1] = nibbleToHex((byte >> 0) & 0xF);
}

}

#endif
