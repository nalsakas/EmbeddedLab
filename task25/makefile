GCC = avr-gcc -g -Os -mmcu=atmega328p
HEX = avr-objcopy
UPLOAD = avrdude -p m328p -c gpio

main.hex: main.elf
	$(HEX) $< -O ihex $@

main.elf: main.o raspnet.o uart.o led.o
	$(GCC) $^ -o $@

main.o: main.c
	$(GCC) -c $<

led.o: led.c led.h
	$(GCC) -c $<

raspnet.o: raspnet.c raspnet.h
	$(GCC) -c $<

uart.o: uart.c uart.h
	$(GCC) -c $<

install: main.hex
	$(UPLOAD) -U flash:w:$<
	
clean:
	rm -fr *.o *.hex *.elf *.exe