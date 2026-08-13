.PHONY: install-kernel-headers

all: $(KERNEL) install-kernel-headers

KERNEL_ARCHDIR := kernel/arch/$(SUBDIR_ARCH)
include $(KERNEL_ARCHDIR)/make.config

KERNEL_CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS) -ffreestanding
KERNEL_CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS) -D__is_kernel
KERNEL_LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
KERNEL_LIBS := $(LIBS) $(KERNEL_ARCH_LIBS) -nostdlib -lk -lgcc

KERNEL_PATH := kernel/kernel
KERNEL_SRCS := $(shell find $(KERNEL_PATH) -type f -name '*.c' -or -name '*.S')

KERNEL_OBJS := $(KERNEL_SRCS:%.c=$(BUILDDIR)/%.o) $(KERNEL_ARCH_OBJS) $(FONT)

LINK_LIST = \
$(KERNEL_LDFLAGS) \
$(KERNEL_OBJS) \
$(KERNEL_LIBS) \

$(KERNEL): $(BUILDDIR)/$(KERNEL_NAME)
	@mkdir -p $(dir $@)
	cp $< $@

$(BUILDDIR)/$(KERNEL_NAME): $(KERNEL_OBJS) $(KERNEL_ARCHDIR)/linker.ld $(LIBK) $(FONT)
	$(CC) -T $(KERNEL_ARCHDIR)/linker.ld -o $@ $(KERNEL_CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot2 $@

# Copy GCC's internal C runtime files into our build directory
$(BUILDDIR)/$(KERNEL_ARCHDIR)/crtbegin.o $(BUILDDIR)/$(KERNEL_ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(KERNEL_CFLAGS) $(KERNEL_LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(BUILDDIR)/kernel/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS)

$(BUILDDIR)/kernel/%.o: kernel/%.S
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS)

install-kernel-headers:
	@mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -r --preserve=timestamps kernel/include/. $(SYSROOT)$(INCLUDEDIR)/.

-include $(KERNEL_OBJS:.o=.d)

