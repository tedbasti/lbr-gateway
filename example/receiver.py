#!/usr/bin/python

import serial
import time
import sys

senderId="\x01"
receiverId="\x00"

#openSerial just opens the serial connection
def openSerial(port):
	#Some configuration for the serial port
	ser = serial.Serial()
	ser.baudrate = 300
	ser.port = port
	ser.bytesize = 8
	ser.stopbits = 2
	ser.open()
	return ser

def initializePort(ser, payloadLen, sender, receiver, layerVersion="\x02"):
	#SenderID
	ser.write(sender)
	#ReceiverID
	ser.write(receiver)
	#layerconfig: At the moment layer2
	ser.write(layerVersion)
	#payloadlen
	ser.write(payloadLen)
	#USART Protocol type: No one reads this field at the moment
	ser.write("\x01")

def main():
	if (len(sys.argv) < 2):
		print "sender.py <port>"
		sys.exit(1)

	layerVersion = "\x02"
	if (len(sys.argv) >= 3):
		layerVersion = "\x03"
		print "Use reliable transport"

	ser = openSerial(sys.argv[1])
	time.sleep(2)
	initializePort(ser, payloadLen="\x01", sender="\x01", receiver="\x00", layerVersion=layerVersion)
	time.sleep(0.5)
	char = 1
	while (char != "\x00"):
		char = ser.read(1)
		sys.stdout.write(char)
		sys.stdout.flush()

	print ""

	while (char != "\x00"):
		char = ser.read(1)
		sys.stdout.write(char)
		sys.stdout.flush()
	ser.close()

if __name__ == '__main__':
    main()
