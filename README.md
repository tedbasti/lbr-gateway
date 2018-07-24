# lbr-gateway
lbr-gateway stands for "low budget radio gateway".
It is designed to run on Atmel ATmega328/P microcontrollers and provides an 433 MHz radio data transmission for client devices.
You can also run lbr-gateway on ATmega328/P based boards like the Arduino Nano, Arduino Uno Rev3, etc.
The client device communicates with lbr-gateway via UART.

# The basic idea
[Client-1] <-UART-> [ATM-1] <-radio 433 MHz-> [ATM-2] <-UART-> [Client-2]

- Client-1/Client-2: Client device communicating with lbr-gateway via UART.
- ATM-1/ATM-2: Atmel ATmega328/P running lbr-gateway which controls 433 MHz radio modules via digital I/O.

Client-1 is the master and Client-2 is the slave. The master client always initiates the data transmission between the master and the slave.
The slave is only allowed to send data when it is requested by the master client.
lbr-gateway might provide the master client to address more than one slave in the future.

# Status of the project
- Client-1 and Client-2 are able to exchange data via a connectionless as well as a connection oriented data transmission protocoll.
- The master-slave protocoll is not yet supported.
- At the moment, the master client as well as the slave client are able to initiate a data transmission.
