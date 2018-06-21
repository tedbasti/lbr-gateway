#!/bin/bash

HEX=${1:-main.hex}
DEV=${ARDUINO_DEV:-$2}

# If you have a default device
#DEV=${DEV:-/dev/arduino_r3}

# Default baudrate for R3,
# for Nano use 57600
BAUD=${3:-115200}
echo "### Flashing ${HEX} ###"

make $HEX
avrdude -F -V -c arduino -p ATMEGA328P -b $BAUD -Uflash:w:${HEX} -P $DEV
