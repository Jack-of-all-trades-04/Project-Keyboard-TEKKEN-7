MCU = atmega32u4
ARCH = AVR8
BOARD = NONE
F_CPU = 16000000

OPTIMIZATION = s
TARGET = TekkenGamepad
SRC = \
	main.c \
	Descriptors.c \
	USBDescriptors.c

LUFA_PATH = ../LUFA

CC = avr-gcc

CFLAGS = -DUSE_LUFA_CONFIG_HEADER -I. -I$(LUFA_PATH)/LUFA/Drivers -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os
LDFLAGS = -Wl,-Map=$(TARGET).map
LDLIBS =

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

clean:
	rm -f *.o *.elf *.hex *.map

flash: $(TARGET).hex
	dfu-programmer $(MCU) erase
	dfu-programmer $(MCU) flash $(TARGET).hex
	dfu-programmer $(MCU) start

.PHONY: all clean flash
