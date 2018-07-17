#!/bin/bash

HEX=lbrgateway.hex
DEV=${ARDUINO_DEV:-$1}

# If you have a default device
#DEV=${DEV:-/dev/arduino_r3}

# Default baudrate for R3,
# for Nano use 57600
# for Uno use 115200
BAUD=${2:-115200}
echo "### Flashing ${HEX} ###"

make $HEX
avrdude -F -V -c arduino -p ATMEGA328P -b $BAUD -Uflash:w:${HEX} -P $DEV
