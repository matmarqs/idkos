# learning NASM

Reference: Nick Blundell, Writing a Simple Operating System from Scratch.

To run the boot sector codes `*.asm` on QEMU:

```
nasm -f bin 1_jmp.asm && qemu-system-i386 -drive format=raw,file=1_jmp
```
