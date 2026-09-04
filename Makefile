# In order to build with this Makefile, you need to have the cross tools in $PATH
# See <https://wiki.osdev.org/GCC_Cross-Compiler#The_Build>
# I am using binutils-2.46.1, gdb-17.2, and gcc-16.1.0.

CC := i686-elf-gcc
LD := i686-elf-ld
AS := nasm

CFLAGS := -Og -g -ffreestanding -std=gnu11 -Wall -Wextra
CPPFLAGS := -Ikernel/include -MD -MP

K_ENTRY := kernel_entry.asm
K_SRCS := $(shell find kernel/ -type f -name '*.c' -or -name '*.asm')
K_OBJS := $(patsubst %.c, %.o, $(patsubst %.asm, %.o, $(K_SRCS)))
K_DEPS := $(K_OBJS:.o=.d)

.PHONY: run clean

run: os.img
	qemu-system-i386 -drive format=raw,file=$<

clean:
	rm -f os.img kernel.bin boot.bin kernel_entry.o $(K_OBJS) $(K_DEPS)

os.img: boot.bin kernel.bin
# The image will be 8192 bytes in size. We first generate 8192 zero bytes,
# then we copy the concatenation of boot.bin and kernel.bin to the image.
	dd bs=512 count=16 if=/dev/zero of=$@ && cat $^ | dd of=$@ conv=notrunc

kernel.bin: kernel_entry.o $(K_OBJS)
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary 2>/dev/null

boot.bin: boot/boot.asm
	$(AS) $< -f bin -o $@ -i boot/

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.asm
	$(AS) $< -f elf -o $@

-include $(K_DEPS)
