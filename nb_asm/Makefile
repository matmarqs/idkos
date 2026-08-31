CC := i686-elf-gcc
LD := i686-elf-ld
ASM := nasm
DISASM := ndisasm

.DELETE_ON_ERROR:
.PHONY: all run disasm clone

all: os.img

run: os.img
	qemu-system-i386  -drive format=raw,file=$<

os.img: boot_sect.bin kernel.bin
	dd bs=512 count=16 if=/dev/zero of=$@ && cat $^ | dd of=$@ conv=notrunc

kernel.bin: kernel_entry.o kernel.o low_level.o
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

boot_sect.bin: boot_sect.asm
	nasm $< -f bin -o $@

%.o: %.c
	$(CC) -ffreestanding -c $< -o $@

%.o: %.asm
	$(ASM) $< -f elf -o $@

clean:
	rm -f *.o *.bin *.img 2>/dev/null
