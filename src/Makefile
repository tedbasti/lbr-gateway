SUBDIRS := usart radio util
SUBLIBS := usart/usart.a radio/radio.a util/util.a
export WORKING_DIR ?= $(PWD)/..
HEX := lbrgateway.hex

all: subdirs $(HEX)

%.hex : %.bin
	avr-objcopy -O ihex -R .eeprom $*.bin $*.hex
	
lbrgateway.bin : $(SUBDIRS) main.cpp
	avr-g++ $(GPP_FLAGS) main.cpp $(SUBLIBS) -o lbrgateway.bin

include $(WORKING_DIR)/subdirs.mk

.PHONY : clean
clean : cleansubdirs
	rm -f *.bin $(HEX) main.o
