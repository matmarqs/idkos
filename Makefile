.PHONY: all clean

BASEPATH != dirname "$(realpath "$(0)")"
export PATH := "$(BASEPATH)/tools/cross/bin:$(PATH)"

export TARGET_ARCH ?= "i686-elf"
export CC := $(TARGET_ARCH)-gcc
export AR := $(TARGET_ARCH)-ar
export AS := $(TARGET_ARCH)-as

export SYSROOT ?= $(shell pwd)/sysroot
export KERNEL := $(SYSROOT)/boot/myos.kernel

# we test on x86_64, but the OS is 32-bit x86
all: myos.iso
	qemu-system-x86_64 -cdrom $<

myos.iso: $(KERNEL) grub.cfg
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub
	cp $(KERNEL) isodir/boot
	grub-mkrescue -o myos.iso isodir

$(KERNEL):
	$(MAKE) -C kernel DESTDIR=$(SYSROOT)

clean:
	find -type f -name '*.o' -or -name '*.d' -or -name '*.a' | xargs rm -f
	rm -rf myos.iso isodir/ $(SYSROOT)
