SUBDIRS := usart util radio
SUBLIBS := usart/usart.a util/util.a radio/radio.a
export WORKING_DIR ?= $(PWD)/..
HEX := lbrgateway.hex

all: subdirs $(HEX)

%.hex : %.bin
	avr-objcopy -O ihex -R .eeprom $*.bin $*.hex
	
lbrgateway.bin : $(SUBDIRS) main.cpp
	avr-g++ $(GPP_FLAGS) main.cpp util/io.o $(SUBLIBS) -o lbrgateway.bin

include $(WORKING_DIR)/subdirs.mk

.PHONY : clean
clean : cleansubdirs
	rm -f *.bin $(HEX) main.o
