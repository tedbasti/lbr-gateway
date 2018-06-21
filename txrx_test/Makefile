SHARED_OBJECTS := usart.o receiver.o transmitter.o timer.o
HEADERS := $(wildcard *.h)
HEX := transmitter.hex receiver.hex
GPP_FLAGS := -I. -O2 -DF_CPU=16000000UL -mmcu=atmega328p

all: $(HEX)

%.hex : %.bin
	avr-objcopy -O ihex -R .eeprom $*.bin $*.hex

transmitter.bin : libshared.a main.cpp
	avr-g++ -DTRANSMITTER $(GPP_FLAGS) main.cpp libshared.a -o transmitter.bin

receiver.bin : libshared.a main.cpp
	avr-g++ -DRECEIVER $(GPP_FLAGS) main.cpp libshared.a -o receiver.bin

libshared.a: $(SHARED_OBJECTS)
	avr-ar crf libshared.a $(SHARED_OBJECTS)

%.o : %.c
	avr-gcc $(GPP_FLAGS) -c -o $*.o $*.c

%.o : %.cpp $(HEADERS)
	avr-g++ $(GPP_FLAGS) -c -o $*.o $*.cpp

.PHONY : clean
clean :
	rm -f *.bin $(HEX) main.o libshared.a $(SHARED_OBJECTS)
