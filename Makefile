# In order to build with this Makefile, you need to have the cross tools in $PATH
# See <https://wiki.osdev.org/GCC_Cross-Compiler#The_Build>
# I am using binutils-2.46.1, gdb-17.2, and gcc-16.1.0.

TARGET_ARCH ?= i686-elf
SUBDIR_ARCH ?= i386

BASEPATH := $(shell pwd)
SYSROOT := $(BASEPATH)/sysroot
BUILDDIR := $(BASEPATH)/build

PREFIX := /usr
BOOTDIR := /boot
INCLUDEDIR := $(PREFIX)/include
LIBDIR := $(PREFIX)/lib

CFLAGS := -O0 -g -std=gnu11 -ffreestanding -Wall -Wextra
CPPFLAGS := -MD -MP -I$(BASEPATH)/libc -I$(BASEPATH)/kernel
LDFLAGS :=
LIBS :=

CC := $(TARGET_ARCH)-gcc
AR := $(TARGET_ARCH)-ar

MAKEFLAGS += -j$(shell nproc)

KERNEL_NAME := myos.kernel
KERNEL := $(SYSROOT)$(BOOTDIR)/$(KERNEL_NAME)
LIBK := $(SYSROOT)$(INCLUDEDIR)/libk.a

.PHONY: all run clean

all: run

# We test on x86_64, but the OS is 32-bit x86
run: myos.iso
	qemu-system-x86_64 -cdrom $<

clean:
	rm -rf myos.iso isodir/ $(SYSROOT) $(BUILDDIR)

myos.iso: $(KERNEL) grub.cfg
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub
	cp $< isodir/boot
	grub-mkrescue -o myos.iso isodir

include $(BASEPATH)/libc/Makefile.inc
include $(BASEPATH)/kernel/Makefile.inc
