TARGET_ARCH ?= i686-elf

CFLAGS ?= -O2 -g
CPPFLAGS ?=
LDFLAGS ?=
LIBS ?=

SYSROOT ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_libc -Iinclude
LIBK_CFLAGS := $(CFLAGS)
LIBK_CPPFLAGS := $(CPPFLAGS) -D__is_libk

ARCHDIR=arch/$(TARGET_ARCH)

include $(ARCHDIR)/make.config

CFLAGS := $(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS := $(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS := $(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREEOBJS = \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdio/abort.o \
stdio/memcmp.o \
stdio/memcpy.o \
stdio/memmove.o \
stdio/memset.o \
stdio/strlen.o \

HOSTEDOBJS = \
$(ARCH_HOSTEDOBJS) \

OBJS = \
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES = libc.a libk.a # Not ready for libc yet
BINARIES = libk.a

.PHONY: all install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES) install-headers install-libs

libc.a: $(OBJS)
	$(AR) rcs $@ $^

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $^

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -r --preserve=timestamps include/. $(SYSROOT)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(SYSROOT)$(LIBDIR)
	cp $(BINARIES) $(SYSROOT)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
