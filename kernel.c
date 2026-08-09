#include "multiboot2.h"
#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef _STDINT
#define _STDINT
#endif

#ifndef _UINT64_T
#define _UINT64_T
#endif

#if defined(__linux__)
#error "You are not using a cross-compiler, __linux__ is defined"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with i686-elf compiler"
#endif

/* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define SSFN_CONSOLEBITMAP_CONTROL /* control chars like '\n' */
#include "ssfn.h"

extern ssfn_font_t _binary_consolefont_sfn_start;

void kernel_main(unsigned long magic, unsigned long addr);

size_t strlen(const char *str);
void term_initialize(uint8_t *fb_addr);
void term_putchar(uint32_t unicode);
void term_write(const char *data, size_t size);
void term_writestring(const char *data);


bool parse_multiboot2(unsigned long magic, unsigned long addr, uint8_t **fb_addr)
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
        }
        /* next multiple by 8: get the 8-byte alignment required by multiboot2 */
        tag = (struct multiboot_tag *) ((uint8_t *) tag + ((tag->size + 7) & ~7));
    }

    return 1;
}

void kernel_main(unsigned long magic, unsigned long addr)
{
    uint8_t *fb_addr = 0;
    parse_multiboot2(magic, addr, &fb_addr);

    term_initialize(fb_addr);
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


void term_initialize(uint8_t *fb_addr) {
    /* set up context by global variables */
    ssfn_src = &_binary_consolefont_sfn_start;  /* the bitmap font to use */
    ssfn_dst.ptr = (uint8_t *) fb_addr;         /* address of the linear frame buffer */
    ssfn_dst.w = OS_WIDTH;                      /* width */
    ssfn_dst.h = OS_HEIGHT;                     /* height */
    ssfn_dst.p = 4096;                          /* bytes per line */
    ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */
    ssfn_dst.bg = 0;                            /* background color */
}


void term_putchar(uint32_t c)
{
    ssfn_putc(c);
}


void term_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        term_putchar(data[i]);
}


void term_writestring(const char *data)
{
    term_write(data, strlen(data));
}
