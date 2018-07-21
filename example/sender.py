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
	ser.baudrate = 9600
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
	if (len(sys.argv) < 3):
		print "sender.py <port> <data>"
		sys.exit(1)

	layerVersion = "\x02"
	if (len(sys.argv) >= 4):
		layerVersion = "\x03"
		print "Use reliable transport"

	ser = openSerial(sys.argv[1])
	time.sleep(2)
	initializePort(ser, payloadLen="\x01", sender=senderId, receiver=receiverId, layerVersion=layerVersion)
	time.sleep(0.5)
	print "sending data"
	ser.write(sys.argv[2])
	ser.flush();
	"""
	Send 4 zeros to verify that one appears
	"""
	print "sending termination byte"
	ser.write("\x00")
	ser.flush()
	"""
	Makes only sense with reliable transport
	"""
	"""
	if (len(sys.argv) >= 5):
		print "printing received Data:"
		while True:
			char = ser.read(1)
			sys.stdout.write(char)
			sys.stdout.flush()
	ser.close()
	"""
	while True:
			char = ser.read(1)
			sys.stdout.write(char)
			sys.stdout.flush()

if __name__ == '__main__':
    main()
