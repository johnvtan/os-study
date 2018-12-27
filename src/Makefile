PREFIX = /home/john/opt/cross/bin/
ARCH = i686-elf
CC = $(PREFIX)/$(ARCH)-gcc
AS = $(PREFIX)/$(ARCH)-as

CFLAGS = -ffreestanding -Wall -Wextra
LDFLAGS = -T linker.ld -ffreestanding -nostdlib -lgcc
ASFLAGS =

KERN_SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.s)
OBJS = $(patsubst %.c, %.o, $(KERN_SOURCES))
OBJS += $(patsubst %.s, %.o, $(ASM_SOURCES))

IMG_NAME = os.bin 

all: build

build: $(OBJS)
	$(CC) -T linker.ld -o $(IMG_NAME) $(LDFLAGS) $(OBJS)
	mv $(IMG_NAME) ../isodir/boot/
	cd .. && grub-mkrescue -o myos.iso isodir

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(ASFLAGS) -c $< -o $@

clean:
	rm $(OBJS)
