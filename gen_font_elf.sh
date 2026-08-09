#!/bin/bash

# First of all, clone https://gitlab.com/bztsrc/scalable-font2
# Compile libsfn and sfnconv with just typing "make"

BASEPATH="$(dirname "$(realpath "$0")")"
BINPATH="$BASEPATH/tools/cross/bin"
LD="$BINPATH/i686-elf-ld"
HOST_FONT_FILE="/usr/share/kbd/consolefonts/default8x16.psfu.gz"
FONT_FILE=$(basename "$HOST_FONT_FILE")

mkdir -p tools
cd tools || exit
cp "$HOST_FONT_FILE" .
./scalable-font2/sfnconv/sfnconv -U -B 16 "$FONT_FILE" consolefont.sfn
"$LD" -r -b binary -o font.o consolefont.sfn
cp font.o "$BASEPATH"
