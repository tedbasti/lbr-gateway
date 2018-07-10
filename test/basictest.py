#!/usr/bin/python

import serial
import time
import sys
import unittest

senderPort = '/dev/ttyUSB0'
receiverPort = '/dev/ttyUSB1'
senderId = "\x00"
receiverId = "\x01"

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

def initializePort(ser, payloadLen, sender, receiver):
	#SenderID
	ser.write(sender)
	#ReceiverID
	ser.write(receiver)
	#layerconfig: At the moment layer2
	ser.write("\x02")
	#payloadlen
	ser.write(payloadLen)
	#USART Protocol type: No one reads this field at the moment 
	ser.write("\x01")
	
	
class BasicTest(unittest.TestCase):
	def setUp(self):
		#Open sender and receiver
		#The sender
		self.serSnd = openSerial(senderPort)
		#The receiver
		self.serRcv = openSerial(receiverPort)
		time.sleep(2)

	def tearDown(self):
		self.serRcv.close()
		self.serSnd.close()
	
	def initialize(self, payloadLen):
		initializePort(self.serSnd, payloadLen, senderId, receiverId)
		initializePort(self.serRcv, payloadLen, receiverId, senderId)
		
		time.sleep(0.5)
	
	def sendBytesAndTestResult(self, payloadLen, stringToWrite, stringToCheck):
		self.initialize(payloadLen)
		
		self.serSnd.write(stringToWrite)
		self.serSnd.flush()
		time.sleep(1)
		message = self.serRcv.read(self.serRcv.inWaiting())
		print message
		#At least one a must be sent!
		self.assertNotEquals(message.find(stringToCheck), -1)
			
	"""
		Test some simple a chars with 
		payloadLen 1.
	"""
	def test_payloadLen01(self):
		self.sendBytesAndTestResult("\x01", "aa", "a")

	def test_payloadLen02(self):
		self.sendBytesAndTestResult("\x02", "aaaa", "aa")

	def test_payloadLen03(self):
		self.sendBytesAndTestResult("\x03", "aaaaaa", "aaa")

	def test_payloadLen04(self):
		self.sendBytesAndTestResult("\x04", "aaaaaaaaaaaa", "aaaa")

	"""This test is to count how much packages
		Will get lost, with some tests
	"""
	def test_sendMuchPackages(self):
		print "Starting sendMuchPackages:"
		self.sendBytesAndTestResult("\x01", "aaaaaaaaaa", "a")

if __name__ == '__main__':
    unittest.main()
