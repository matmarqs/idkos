# learning NASM

Reference: Nick Blundell, Writing a Simple Operating System from Scratch.

To run the boot sector codes `*.asm` on QEMU:

```
nasm -f bin 1_jmp.asm && qemu-system-i386 -drive format=raw,file=1_jmp
```

# compiling freestanding and disassembly

```
export OBJFILE=basic.o
i686-elf-gcc -ffreestanding -c "${OBJFILE%.o}.c" -o "$OBJFILE"
objdump -M intel -d "$OBJFILE"
i686-elf-ld -o "${OBJFILE%.o}.bin" -Ttext 0x0 --oformat binary "$OBJFILE"
ndisasm -b 32 "${OBJFILE%.o}.bin"
```

# generating kernel image

```
i686-elf-gcc -ffreestanding -c kernel.c -o kernel.o
i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary
nasm -f bin boot_sect.asm -o boot_sect.bin
cat boot_sect.bin kernel.bin > os.img
dd bs=512 count=16 if=/dev/zero of=os-image.img
dd bs=512 if=os.img of=os-image.img conv=notrunc
wc -c os-image.img                  # Needs to have 8192 bytes (16 sectors)
qemu-system-i386 os-image.img
```
