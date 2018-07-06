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
	#SenderID: Sender is "Master"
	ser.write("\x00")
	#ReceiverID: Receiver has id 1
	ser.write("\x01")
	#layerconfig: At the moment layer2
	ser.write("\x02")
	#payloadlen: Only one at the moment
	ser.write("\x01")
	#USART Protocol type: isnt read at the moment
	ser.write("\x01")
	
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
		time.sleep(0.5)
		
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
