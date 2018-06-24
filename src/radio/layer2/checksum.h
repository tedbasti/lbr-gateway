#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>

class XorChecksum8 {
public:
	XorChecksum8() {
		reset();
	}
	
	void reset() {
		digest = 0;
	}
	
	void addByte(uint8_t byte) {
		digest ^= byte;
	}
	
	void addBytes(uint8_t const *bytes, uint8_t len) {
		for (uint8_t i = 0; i < len; ++i) {
			addByte(bytes[i]);
		}
	}
	
	uint8_t getDigest() const {
		return digest;
	}
	
	bool isValid() const {
		return digest == 0;
	}
private:
		uint8_t digest;
};

#endif
