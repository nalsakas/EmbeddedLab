GCC = avr-gcc -g -Os -mmcu=atmega328p
HEX = avr-objcopy
UPLOAD = avrdude -p m328p -c gpio

main.hex: main.elf
	$(HEX) $< -O ihex $@

main.elf: main.o raspnetL1.o raspnetL2.o uart.o crc.o led.o
	$(GCC) $^ -o $@

main.o: main.c
	$(GCC) -c $<

led.o: led.c led.h
	$(GCC) -c $<

raspnetL1.o: raspnetL1.c raspnetL1.h
	$(GCC) -c $<

raspnetL2.o: raspnetL2.c raspnetL2.h
	$(GCC) -c $<

uart.o: uart.c uart.h
	$(GCC) -c $<

crc.o: crc.c crc.h
	$(GCC) -c $<

install: main.hex
	$(UPLOAD) -U flash:w:$<
	
clean:
	rm -fr *.o *.hex *.elf *.exe