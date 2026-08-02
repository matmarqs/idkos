#!/bin/bash

nasm -f bin -o boot.bin -D NUM_SECTORS=1 stage-1.asm && \
dd if=/dev/zero of=disk.img bs=512 count=2880 && \
dd if=boot.bin of=disk.img conv=notrunc && \
dd if=stage-2.bin of=disk.img bs=512 seek=1 conv=notrunc && \
echo "Image disk.img created!"
