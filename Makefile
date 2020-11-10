# ----------------
# C
# ----------------
C_SOURCES_DIRS = kernel drivers utils
C_HEADERS_DIRS = kernel drivers utils

C_SOURCES = $(shell find $(C_SOURCES_DIRS) -name '*.c')
C_HEADERS = $(shell find $(C_HEADERS_DIRS) -name '*.h')

C_OBJ = ${C_SOURCES:.c=.o}

# ----------------
# Assembly
# ----------------
ASM_SOURCES_DIRS = kernel drivers utils # Not including the boot assembly file.
ASM_SOURCES = $(shell find $(ASM_SOURCES_DIRS) -name '*.asm')
ASM_OBJ = ${ASM_SOURCES:.asm=.o}

# ----------------
# Flags
# ----------------
GCC_DEBUG_FLAG = -g
GCC_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c -O1
GCC_LINK_FLAGS = -T config/linker.ld -ffreestanding -O2 -nostdlib $^ -lgcc

NASM_FLAGS = -felf32

QEMU_FLAGS = -serial file:serial.log
QEMU_DEBUG_FLAGS = -s -S

# ----------------
# Paths
# ----------------
COMPILERS_DIR_PATH = ~/opt/cross/bin
GCC_PATH = ${COMPILERS_DIR_PATH}/./i686-elf-gcc
GNU_ASSEMBLER_PATH = ${COMPILERS_DIR_PATH}/./i686-elf-as

NASM_PATH = nasm

# ----------------
# Rules
# ----------------
all: os.iso

run: os.iso
	qemu-system-i386 ${QEMU_FLAGS} -cdrom $<

debug: os.iso
	qemu-system-i386 ${QEMU_FLAGS} ${QEMU_DEBUG_FLAGS} -cdrom $<

# ----------------
# Automation
# ----------------
os.iso: os.bin config/grub.cfg
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$<
	cp config/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

# ----------------
# Compile
# ----------------
os.bin: boot/boot.o ${ASM_OBJ} ${C_OBJ}
	${GCC_PATH} -o $@ ${GCC_LINK_FLAGS} ${GCC_DEBUG_FLAG}

boot/boot.o: boot/boot.s
	${GNU_ASSEMBLER_PATH} boot/boot.s -o $@

%.o : %.c ${C_HEADERS} # Depends on the c file, and all the c headers.
	${GCC_PATH} $< -o $@ ${GCC_FLAGS} ${GCC_DEBUG_FLAG}

%.o: %.asm
	${NASM_PATH} ${NASM_FLAGS} $< -o $@

# ----------------
# Clean
# ----------------
clean:
	find . -name \*.o -type f -delete
	find . -name \*.bin -type f -delete 
	rm -rf -f isodir
	rm -f os.iso
	rm -f *.log