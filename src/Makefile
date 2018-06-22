SUBDIRS := usart radio util
export WORKING_DIR ?= $(PWD)/..
export BUILD_DIR ?= $(WORKING_DIR)/build
HEX := transmitter.hex receiver.hex

all: subdirs $(HEX)

%.hex : %.bin
	avr-objcopy -O ihex -R .eeprom $*.bin $*.hex

transmitter.bin : $(SUBDIRS) main.cpp
	avr-g++ -DTRANSMITTER $(GPP_FLAGS) main.cpp $(BUILD_DIR)/usart.a $(BUILD_DIR)/radio.a $(BUILD_DIR)/util.a -o transmitter.bin

receiver.bin : $(SUBDIRS) main.cpp
	avr-g++ -DRECEIVER $(GPP_FLAGS) main.cpp $(BUILD_DIR)/usart.a $(BUILD_DIR)/radio.a $(BUILD_DIR)/util.a  -o receiver.bin

include $(WORKING_DIR)/subdirs.mk

.PHONY : clean
clean : cleansubdirs
	rm -f *.bin $(HEX) main.o
