#include "multiboot2.h"

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
#include "ssfn.h"

extern ssfn_font_t _binary_consolefont_sfn_start;

void kernel_main(unsigned long magic, unsigned long addr);

size_t strlen(const char *str);
void terminal_initialize(uint8_t *fb_addr);
void terminal_putchar(uint32_t unicode);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);


void parse_multiboot2(unsigned long addr)
{
    addr = addr + 0;
    return;
}


void kernel_main(unsigned long magic, unsigned long addr)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        ;
    }
    parse_multiboot2(addr);
    return;
}


size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}


void terminal_initialize(uint8_t *fb_addr) {
    /* set up context by global variables */
    ssfn_src = &_binary_consolefont_sfn_start;      /* the bitmap font to use */
    ssfn_dst.ptr = (uint8_t *) fb_addr;         /* address of the linear frame buffer */
    ssfn_dst.w = 1024;                          /* width */
    ssfn_dst.h = 768;                           /* height */
    ssfn_dst.p = 4096;                          /* bytes per line */
    ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */
    ssfn_dst.bg = 0;                            /* background color */
}


void terminal_putchar(uint32_t unicode)
{
    ssfn_putc(unicode);
}


void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}


void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}
