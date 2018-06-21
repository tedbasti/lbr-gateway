#!/bin/bash

DEV=${ARDUINO_DEV:-$2}

# If you have a default device
#DEV=${DEV:-/dev/arduino_r3}

stty -F $DEV cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

echo -n ${1} > $DEV
