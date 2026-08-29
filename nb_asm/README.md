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
