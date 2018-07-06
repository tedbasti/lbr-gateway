#!/usr/bin/python

import serial
import time
import sys
import unittest

senderPort = '/dev/ttyUSB0'
receiverPort = '/dev/ttyUSB1'

#some helper functions
#openSerial just opens the serial connection
def openSerial(port):
	#Some configuration for the serial port
	ser = serial.Serial()
	ser.baudrate = 57600
	ser.port = port
	ser.bytesize = 8
	ser.stopbits = 2
	ser.open()
	return ser

#InitializeSender will be for the usart initialization
def initializeSender(ser):
	#At the moment no initialization needed
	pass
	
class BasicTest(unittest.TestCase):
	def setUp(self):
		#Open sender and receiver
		#The sender
		self.serSnd = openSerial(senderPort)
		#The receiver
		self.serRcv = openSerial(receiverPort)
		time.sleep(2)
		#Initialize the sender
		initializeSender(self.serSnd)
		
	def tearDown(self):
		self.serRcv.close()
		self.serSnd.close()
		
	def test_basic(self):
		self.serSnd.write("aaaaa")
		self.serSnd.flush()
		time.sleep(1)		
		message = self.serRcv.read(self.serRcv.inWaiting())
		#At least one a must be sent!
		self.assertNotEquals(message.find('a'), -1)

if __name__ == '__main__':
    unittest.main()
