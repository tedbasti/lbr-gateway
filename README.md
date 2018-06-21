# lbr-gateway

lbr-gateway stands for "low budged radio module gateway".
This is a kind of library/gateway for some low budged 433MHz transmitter/receiver.
You need an atmega328 microcontroller for this gateway (or use an arduino nano/uno).
The target is, that you connect two microcontrollers (together with this gateway) over radio.

# Status of the project 
This project is not useable at the moment.

# The basic idea
(UC1) <-USART-> (ATM) <-radio 433MHz-> (ATM) <-USART-> (UC2)

- UC1/UC2: Some microcontroller (need to be able to speak USART)
- ATM: atmega328 from atmel
- UC1/UC2 and ATM talks with USART
- The two atmegas talking (with some radio module) about 433MHz

One of the UCs is the master and the other UCs are slaves (might be more than two in future).
The master is always allowed to send. The slave is only allowed to send, when asked by a master.
e.g. when UC1 is master, then UC2 is only allowed to send data over radio, when UC1 asked for some data.
(The master/slave idea is taken from modbus!)


# TODOs for the Readme
Link some radio modules
