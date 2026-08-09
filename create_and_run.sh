#!/bin/bash

BASEPATH="$(dirname "$(realpath "$0")")"
BINPATH="$BASEPATH/tools/cross/bin"
CC="$BINPATH/i686-elf-gcc"

cd "$BASEPATH" && \
"$CC" -c boot.S -o boot.o -I. && \
"$CC" -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && \
"$CC" -T linker.ld -o myos -ffreestanding -O2 -nostdlib boot.o kernel.o font.o -lgcc && \
grub-file --is-x86-multiboot2 myos && echo "myos created and is x86 multiboot2 bootable" && \
cp myos isodir/boot && \
grub-mkrescue -o myos.iso isodir && \
qemu-system-x86_64 -cdrom myos.iso
