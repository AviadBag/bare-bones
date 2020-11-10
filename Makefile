C_SOURCES_DIRS = kernel drivers utils
C_HEADERS_DIRS = kernel drivers utils

C_SOURCES = $(shell find $(C_SOURCES_DIRS) -name '*.c')
C_HEADERS = $(shell find $(C_HEADERS_DIRS) -name '*.h')

C_OBJ = ${C_SOURCES:.c=.o}

GCC_DEBUG_FLAG = -g
GCC_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c -O1
GCC_LINK_FLAGS = -T config/linker.ld -ffreestanding -O2 -nostdlib $^ -lgcc

QEMU_FLAGS = -serial file:serial.log
QEMU_DEBUG_FLAGS = -s -S

all: os.iso

run: os.iso
	qemu-system-i386 ${QEMU_FLAGS} -cdrom $<

debug: os.iso
	qemu-system-i386 ${QEMU_FLAGS} ${QEMU_DEBUG_FLAGS} -cdrom $<

os.iso: os.bin config/grub.cfg
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$<
	cp config/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

os.bin: boot/boot.o ${C_OBJ}
	~/opt/cross/bin/./i686-elf-gcc -o $@ ${GCC_LINK_FLAGS} ${GCC_DEBUG_FLAG}

boot/boot.o: boot/boot.s
	~/opt/cross/bin/./i686-elf-as boot/boot.s -o $@

%.o : %.c ${C_HEADERS}
	~/opt/cross/bin/./i686-elf-gcc $< -o $@ ${GCC_FLAGS} ${GCC_DEBUG_FLAG}

clean:
	find . -name \*.c_o -type f -delete 
	find . -name \*.asm_o -type f -delete 
	find . -name \*.bin -type f -delete 
	rm -rf -f isodir
	rm -f os.iso
	rm -f *.log