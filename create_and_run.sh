#!/bin/bash

BASEPATH="$(dirname $(realpath "$0"))"
BINPATH="$BASEPATH/tools/cross/bin"
CC="$BINPATH/i686-elf-gcc"
AS="$BINPATH/i686-elf-as"

cd $BASEPATH && \
"$AS" boot.s -o boot.o && \
"$CC" -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && \
"$CC" -T linker.ld -o myos -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc && \
grub-file --is-x86-multiboot myos && echo "myos created and is x86 bootable" && \
cp myos isodir/boot && grub-mkrescue -o myos.iso isodir && \
qemu-system-i386 -cdrom myos.iso
