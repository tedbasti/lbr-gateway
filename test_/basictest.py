#!/usr/bin/python

import serial
import time
import sys

def openSerial(port):
	#Some configuration for the serial port
	ser = serial.Serial()
	ser.baudrate = 57600
	ser.port = port
	ser.bytesize = 8
	ser.stopbits = 2
	ser.open()
	return ser

def initializeSender(ser):
	#At the moment no initialization needed
	pass

#Open sender and receiver
#The sender
serSnd = openSerial(sys.argv[1])
#The receiver
serRcv = openSerial(sys.argv[2])
time.sleep(2)
#Initialize the sender
initializeSender(serSnd)

serSnd.write("aaaaa")
serSnd.flush()
time.sleep(1)

message = serRcv.read(serRcv.inWaiting())
if message.find('a') == -1:
	print "sign not found! FAILURE"
	sys.exit(1)

serRcv.close()
serSnd.close()
sys.exit(0)
