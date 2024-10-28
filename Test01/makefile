GCC = avr-gcc -g -Os -mmcu=atmega328p
HEX = avr-objcopy
UPLOAD = avrdude -p m328p -c gpio

main.hex: main.elf
	$(HEX) $< -O ihex $@

main.elf: main.o uart.o api.o pins.o	
	$(GCC) $^ -o $@

main.o: main.c
	$(GCC) -c $<

uart.o: uart.c uart.h
	$(GCC) -c $<

api.o: api.c api.h
	$(GCC) -c $<

pins.o: pins.c pins.h
	$(GCC) -c $<

install: main.hex
	$(UPLOAD) -U flash:w:$<

doc:
	doxygen
	
clean:
	rm -fr *.o *.hex *.elf  *.exe