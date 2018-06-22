SUBDIRS := usart radio util
SUBLIBS := usart/usart.a radio/radio.a util/util.a
export WORKING_DIR ?= $(PWD)/..
HEX := transmitter.hex receiver.hex

all: subdirs $(HEX)

%.hex : %.bin
	avr-objcopy -O ihex -R .eeprom $*.bin $*.hex

transmitter.bin : $(SUBDIRS) main.cpp
	avr-g++ -DTRANSMITTER $(GPP_FLAGS) main.cpp $(SUBLIBS) -o transmitter.bin

receiver.bin : $(SUBDIRS) main.cpp
	avr-g++ -DRECEIVER $(GPP_FLAGS) main.cpp $(SUBLIBS) -o receiver.bin

include $(WORKING_DIR)/subdirs.mk

.PHONY : clean
clean : cleansubdirs
	rm -f *.bin $(HEX) main.o
