CC := i686-elf-gcc
LD := i686-elf-ld
AS := nasm

.PHONY: run clean

run: boot.bin
	qemu-system-i386 -drive format=raw,file=$<

clean:
	rm -f boot.bin

boot.bin: boot/boot.asm
	nasm $< -f bin -o $@ -i boot/

%.o: %.c
	$(CC) -ffreestanding -c $< -o $@

%.o: %.asm
	$(AS) $< -f elf -o $@
