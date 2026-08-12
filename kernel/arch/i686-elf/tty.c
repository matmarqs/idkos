/* tty.c: here we put the term_* functions to print to the screen */
#include <stdint.h>
#include <string.h>

#ifndef _STDINT
#define _STDINT
#endif

#ifndef _UINT64_T
#define _UINT64_T
#endif

/* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define SSFN_CONSOLEBITMAP_CONTROL /* control chars like '\n' */
#include "ssfn.h"

extern ssfn_font_t _binary_consolefont_sfn_start;

void term_initialize(void)
{
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
    for (size_t i = 0; i < size; i++)
        term_putchar(data[i]);
}

void term_writestring(const char *data)
{
    term_write(data, strlen(data));
}
