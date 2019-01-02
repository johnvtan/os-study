PREFIX = /home/john/opt/cross/bin/
ARCH = i686-elf
CC = $(PREFIX)/$(ARCH)-gcc
AS = $(PREFIX)/$(ARCH)-as

CFLAGS = -ffreestanding -Wall -Wextra
LDFLAGS = -T linker.ld -ffreestanding -nostdlib -lgcc
ASFLAGS =

OBJ_DIR = objs
KERN_SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.s)
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(KERN_SOURCES))
OBJS += $(patsubst %.s, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

IMG_NAME = os.bin 

all: dirs build

dirs:
	mkdir -p $(OBJ_DIR)

build: $(OBJS)
	$(CC) -T linker.ld -o $(IMG_NAME) $(LDFLAGS) $(OBJS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.s
	$(CC) $(ASFLAGS) -c $< -o $@

upload: dirs build
	mv $(IMG_NAME) ../isodir/boot/
	cd .. && grub-mkrescue -o myos.iso isodir

clean:
	rm -rf $(OBJ_DIR) $(IMG_NAME)
