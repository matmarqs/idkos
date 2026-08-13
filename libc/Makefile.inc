.PHONY: install-libc install-libc-headers

BINARIES := $(BUILDDIR)/libk.a # $(BUILDDIR)/libc.a # Not ready for libc yet

all: $(BINARIES)

LIBC_ARCHDIR := libc/arch/$(SUBDIR_ARCH)
include $(LIBC_ARCHDIR)/make.config

LIBK_CFLAGS := $(CFLAGS) $(LIBK_ARCH_CFLAGS)
LIBK_CPPFLAGS := $(CPPFLAGS) $(LIBK_ARCH_CPPFLAGS) -D__is_libk

LIBC_CFLAGS := $(CFLAGS) $(LIBC_ARCH_CFLAGS)
LIBC_CPPFLAGS := $(CPPFLAGS) $(LIBC_ARCH_CPPFLAGS) -D__is_libc

COMMON_PATH := libc/common
COMMON_SRCS := $(shell find $(COMMON_PATH) -type f -name '*.c')

HOSTED_PATH := libc/hosted
HOSTED_SRCS := $(shell find $(HOSTED_PATH) -type f -name '*.c')

LIBK_SRCS := $(COMMON_SRCS) $(LIBK_ARCH_SRCS)
LIBC_SRCS := $(COMMON_SRCS) $(HOSTED_SRCS) $(LIBK_ARCH_SRCS) $(LIBC_ARCH_SRCS)

LIBK_OBJS=$(LIBK_SRCS:%.c=$(BUILDDIR)/%.libk.o)
LIBC_OBJS=$(LIBC_SRCS:%.c=$(BUILDDIR)/%.libc.o)

$(BUILDDIR)/libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $^

$(BUILDDIR)/libc.a: $(LIBC_OBJS)
	$(AR) rcs $@ $^

$(BUILDDIR)/libc/%.libk.o: libc/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

$(BUILDDIR)/libc/%.libc.o: libc/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(LIBC_CFLAGS) $(LIBC_CPPFLAGS)

install-libc-headers:
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -r --preserve=timestamps include/. $(SYSROOT)$(INCLUDEDIR)/.

install-libc: $(BINARIES)
	mkdir -p $(SYSROOT)$(LIBDIR)
	cp $(BINARIES) $(SYSROOT)$(LIBDIR)

-include $(LIBC_OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
