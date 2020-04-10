MCU=atmega328p
PORT=$(shell pavr2cmd --prog-port)
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) -Os

CC=avr-gcc
TARGET=simple_melt

AVRDUDE_DEVICE = m328p
AVRDUDE=avrdude
DEVICE ?= atmega168
OBJECT_FILES=simple_melt.o
OBJ2HEX=avr-objcopy
LDFLAGS=-Wl,-gc-sections -lpololu_$(DEVICE) -Wl,-relax

all: $(TARGET).hex

clean:
	rm -f *.o *.elf *.hex

%.hex: %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

simple_melt.o: simple_melt.cpp

%.obj: $(OBJECT_FILES)
		$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
		$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex
