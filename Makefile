APP = blink
PORT = /dev/ttyACM0

all:
	avr-gcc ${APP}.c -o ${APP}.elf -mmcu=atmega328p -DF_CPU=16000000UL -Os
	avr-objcopy ${APP}.elf -O ihex ${APP}.hex
	avrdude -c arduino -p m328p -P ${PORT} -U flash:w:${APP}.hex:i -D
	rm *.hex *.elf

build:
	avr-gcc ${APP}.c -o ${APP}.elf -mmcu=atmega328p -DF_CPU=16000000UL -Os
	avr-objcopy ${APP}.elf -O ihex ${APP}.hex

flash:	
	avrdude -c arduino -p m328p -P ${PORT} -U flash:w:${APP}.hex:i -D

clean:
	rm *.hex *.elf 
