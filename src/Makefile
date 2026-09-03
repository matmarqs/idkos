CC := i686-elf-gcc
LD := i686-elf-ld
AS := nasm

.PHONY: run clean

run: os.img
	qemu-system-i386 -drive format=raw,file=$<

clean:
	rm -f boot.bin

os.img: boot.bin kernel.bin
# The image will be 8192 bytes in size. We first generate 8192 zero bytes,
# then we copy the concatenation of boot.bin and kernel.bin to the image.
	dd bs=512 count=16 if=/dev/zero of=$@ && cat $^ | dd of=$@ conv=notrunc

kernel.bin:
	touch $@

boot.bin: boot/boot.asm
	nasm $< -f bin -o $@ -i boot/

%.o: %.c
	$(CC) -ffreestanding -c $< -o $@

%.o: %.asm
	$(AS) $< -f elf -o $@
