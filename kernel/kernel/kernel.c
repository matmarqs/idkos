#include "multiboot2.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#if defined(__linux__)
#error "You are not using a cross-compiler, __linux__ is defined"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with i686-elf compiler"
#endif


void kernel_main(unsigned long magic, unsigned long addr);

size_t strlen(const char *str);


bool parse_multiboot2(unsigned long magic, unsigned long addr,
                      uint8_t **fb_addr, uint32_t *fb_width,
                      uint32_t *fb_height, uint32_t *fb_pitch)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        /* Invalid magic number */
        return 0;
    }

    if (addr & 7) {
        /* Unaligned mbi */
        return 0;
    }

    struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            struct multiboot_tag_framebuffer *tagfb =
                (struct multiboot_tag_framebuffer *) tag;
            *fb_addr = (uint8_t *) (unsigned long) tagfb->common.framebuffer_addr;
            *fb_width = tagfb->common.framebuffer_width;
            *fb_height = tagfb->common.framebuffer_height;
            *fb_pitch = tagfb->common.framebuffer_pitch;
        }
        /* next multiple by 8: get the 8-byte alignment required by multiboot2 */
        tag = (struct multiboot_tag *) ((uint8_t *) tag + ((tag->size + 7) & ~7));
    }

    return 1;
}

void kernel_main(unsigned long magic, unsigned long addr)
{
    uint8_t *fb_addr = 0;
    uint32_t fb_width = 0;
    uint32_t fb_height = 0;
    uint32_t fb_pitch = 0;

    parse_multiboot2(magic, addr, &fb_addr, &fb_width, &fb_height, &fb_pitch);

    term_initialize(fb_addr, fb_width, fb_height, fb_pitch);
    term_writestring("Hello world\r\n");

    return;
}


size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}
