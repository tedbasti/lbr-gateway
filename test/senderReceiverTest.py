#!/usr/bin/python

import serial
import time
import sys
import unittest

senderPort = '/dev/ttyUSB0'

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
def initializeSender(ser, payloadLen):
	#SenderID: Sender is "Master"
	ser.write("\x00")
	#ReceiverID: Receiver has id 0
	ser.write("\x00")
	#layerconfig: At the moment layer2
	ser.write("\x02")
	#payloadlen: Only one at the moment
	#ser.write("\x01")
	ser.write(payloadLen)
	#USART Protocol type: isnt read at the moment
	ser.write("\x01")
	
class BasicTest(unittest.TestCase):
	def setUp(self):
		#Open sender and receiver
		#The sender
		self.serSnd = openSerial(senderPort)
		time.sleep(2)

	def tearDown(self):
		self.serSnd.close()
	
	def initialize(self, payloadLen):
		initializeSender(self.serSnd, payloadLen)
		time.sleep(0.5)
	
	"""This test is to count how much packages
		Will get lost, with some tests
		This will use the sender and receiver as one!
		Means that sender and receiverId must be the same!
		Sounds weird but is a valid test:D
	"""
	def test_sendMuchPackages(self):
		print "Starting sendMuchPackages:"
		self.initialize("\x01")
		
		self.serSnd.write("aaaaaaaaaa")
		self.serSnd.flush()
		time.sleep(1)
		message = self.serSnd.read(self.serSnd.inWaiting())
		print message

if __name__ == '__main__':
    unittest.main()
