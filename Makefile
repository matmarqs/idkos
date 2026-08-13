# In order to build with this Makefile, you need to have the cross tools in $PATH
# See <https://wiki.osdev.org/GCC_Cross-Compiler#The_Build>
# I am using binutils-2.46.1, gdb-17.2, and gcc-16.1.0.

TARGET_ARCH ?= i686-elf
SUBDIR_ARCH ?= i386

SYSROOT := sysroot
BUILDDIR := build

PREFIX := /usr
BOOTDIR := /boot
INCLUDEDIR := $(PREFIX)/include
LIBDIR := $(PREFIX)/lib

CFLAGS := -O0 -g -std=gnu11 -Wall -Wextra
CPPFLAGS := -MD -MP -Ilibc/include -Ikernel/include
LDFLAGS := -L$(SYSROOT)$(LIBDIR)
LIBS :=

CC := $(TARGET_ARCH)-gcc
AR := $(TARGET_ARCH)-ar
LD := $(TARGET_ARCH)-ld

MAKEFLAGS += -j$(shell nproc)

KERNEL_NAME := myos.kernel
KERNEL := $(SYSROOT)$(BOOTDIR)/$(KERNEL_NAME)
LIBK := $(SYSROOT)$(LIBDIR)/libk.a

FONT := $(BUILDDIR)/font.o
FONT_PATH := "/usr/share/kbd/consolefonts/default8x16.psfu.gz"

.PHONY: all run clean

myos.iso: $(KERNEL) grub.cfg
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub
	cp $< isodir/boot
	grub-mkrescue -o myos.iso isodir

all: myos.iso

# We test on x86_64, but the OS is 32-bit x86
run: myos.iso
	qemu-system-x86_64 -cdrom $<

clean:
	rm -rf myos.iso isodir/ $(SYSROOT) $(BUILDDIR)

# Be careful: `$LD` generates the symbol _binary_<ARG>_sfn_start
# based on its last argument <ARG>
$(FONT): $(BUILDDIR)/consolefont.sfn
	cd $(BUILDDIR) && $(LD) -r -b binary -o font.o consolefont.sfn

$(BUILDDIR)/consolefont.sfn:
	sfnconv -U -B 16 $(FONT_PATH) $@

include libc/Makefile.inc
include kernel/Makefile.inc
