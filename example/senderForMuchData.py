#!/usr/bin/python

import serial
import time
import sys

senderId = "\x00"
receiverId = "\x01"

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
	if (len(sys.argv) < 4):
		print "sender.py <port> <layerVersion> <data> [<payloadLen=1>]"
		sys.exit(1)

	layerVersion = "\x02"
	if (sys.argv[2] == "3"):
		layerVersion = "\x03"
		print "Layer version 3!"
	elif (sys.argv[2] == "2"):
		layerVersion = "\x02"
		print "Layer version 2!"
	else:
		print "invalid layer version!"
		sys.exit(1)


	pLen = "\x01"
	if (len(sys.argv) >= 5):
		pLen = hex(int(sys.argv[4]))

	ser = openSerial(sys.argv[1])
	time.sleep(2)
	initializePort(ser, payloadLen=pLen, sender=senderId, receiver=receiverId, layerVersion=layerVersion)
	time.sleep(0.5)
	print "sending data"
	for x in sys.argv[3]:
		ser.write(x)
		ser.flush()
		time.sleep(1)

	print "sending termination byte"
	ser.write("\x00")
	ser.flush()
	while True:
		char = ser.read(1)
		sys.stdout.write(char)
		sys.stdout.flush()

if __name__ == '__main__':
    main()
