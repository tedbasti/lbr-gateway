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
	ser.baudrate = 1200
	ser.port = port
	ser.bytesize = 8
	ser.stopbits = 2
	ser.timeout = 5
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

	def initialize(self, payloadLen, layerVersion="\x02"):
		initializePort(self.serSnd, payloadLen, senderId, receiverId, layerVersion)
		initializePort(self.serRcv, payloadLen, receiverId, senderId, layerVersion)

		time.sleep(0.5)

	def sendBytesAndTestResult(self, payloadLen, stringToWrite, stringToCheck, layerVersion="\x02", waitForAllPackages=False):
		self.initialize(payloadLen, layerVersion)

		self.serSnd.write(stringToWrite)
		self.serSnd.flush()
		time.sleep(1)
		#if not waitForAllPackages:
		#	message = self.serRcv.read(self.serRcv.inWaiting())
		#else:
		message = self.serRcv.read(len(stringToCheck))
		print message
		#Just to delete everything from the buffer
		self.serRcv.read(self.serRcv.inWaiting())
		#At least one a must be sent!
		self.assertNotEquals(message.find(stringToCheck), -1)

	"""
		Test some simple a chars with
		payloadLen 1.
	"""
	def test_payloadLen01(self):
		print "test_payloadLen01"
		self.sendBytesAndTestResult("\x01", "aa", "a")

	def test_payloadLen02(self):
		print "test_payloadLen02"
		self.sendBytesAndTestResult("\x02", "abab", "ab")

	def test_payloadLen03(self):
		print "test_payloadLen03"
		self.sendBytesAndTestResult("\x03", "abcabc", "abc")

	def test_payloadLen04(self):
		print "test_payloadLen04"
		self.sendBytesAndTestResult("\x04", "abcdabcdaaaa", "abcd")

	def test_payloadLen01_layer3(self):
		print "test_payloadLen01_layer3"
		val="a"
		self.sendBytesAndTestResult("\x01", val, "a", "\x03")
#		message = self.serSnd.read(1)
#		print "sender: " + message

	def test_payloadLen01_layer3_SameLetter(self):
		print "test_payloadLen01_layer3_SameLetter"
		val="aaaaa"
		self.sendBytesAndTestResult("\x01", val, val, "\x03", True)

	def test_payloadLen01_layer3_much(self):
		print "test_payloadLen01_layer3_much"
		val="abcde"
		self.sendBytesAndTestResult("\x01", val, val, "\x03", True)

	"""This test is to count how much packages
		Will get lost, with some tests
	"""
	def test_sendMuchPackages(self):
		print "test_sendMuchPackages"
		self.sendBytesAndTestResult("\x01", "aaaaaaaaaa", "a")

if __name__ == '__main__':
    unittest.main()
