GPP_FLAGS := -std=c++11 -I. -O2 -DF_CPU=16000000UL -mmcu=atmega328p

%.o : %.c
	avr-gcc $(GPP_FLAGS) -c -o $*.o $*.c

%.o : %.cpp
	avr-g++ $(GPP_FLAGS) -c -o $*.o $*.cpp