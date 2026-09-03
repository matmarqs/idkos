/* tty.c: here we put the term_* functions to print to the screen */
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <3rdparty/multiboot2.h>

#define _STDINT
#define _UINT64_T
/* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define SSFN_CONSOLEBITMAP_CONTROL /* control chars like '\n' */
#include <3rdparty/ssfn.h>
extern ssfn_font_t _binary_consolefont_sfn_start;


static bool parse_multiboot2(unsigned long magic, unsigned long addr,
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

void term_initialize(unsigned long magic, unsigned long addr) {
    uint8_t *fb_addr = 0;
    uint32_t fb_width = 0;
    uint32_t fb_height = 0;
    uint32_t fb_pitch = 0;

    parse_multiboot2(magic, addr, &fb_addr, &fb_width, &fb_height, &fb_pitch);

    /* set up context by global variables */
    ssfn_src = &_binary_consolefont_sfn_start;  /* the bitmap font to use */
    ssfn_dst.ptr = (uint8_t *) fb_addr;         /* address of the linear frame buffer */
    ssfn_dst.w = fb_width;                      /* width */
    ssfn_dst.h = fb_height;                     /* height */
    ssfn_dst.p = fb_pitch;                      /* bytes per line */
    ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */
    ssfn_dst.bg = 0;                            /* background color */
}

void term_putchar(char c)
{
    ssfn_putc((char) c);
}

void term_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        term_putchar(data[i]);
    }
}

void term_writestring(const char *data) {
    size_t len = 0;
    while (data[len]) {
        term_putchar(data[len++]);
    }
}
