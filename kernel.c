#include "multiboot2.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, __linux__ defined"
#endif

#if !defined(__i386__)
#error "This tutorial needs to be compiled with i686-elf compiler"
#endif

/* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <ssfn.h>

/* set up context by global variables */
ssfn_src = &_binary_console_sfn_start;      /* the bitmap font to use */

ssfn_dst.ptr = 0xE0000000;                  /* address of the linear frame buffer */
ssfn_dst.w = 1024;                          /* width */
ssfn_dst.h = 768;                           /* height */
ssfn_dst.p = 4096;                          /* bytes per line */
ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */

void kernel_main(unsigned long magic, unsigned long addr);
void printf(const char *format, ...);

void kernel_main(unsigned long magic, unsigned long addr)
{
    struct multiboot_tag *tag;
    unsigned size;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer = (uint16_t *) VGA_MEMORY;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}


void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
        return;
    }
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_print_uint32(uint32_t x) {
    if (x == 0) {
        terminal_putchar('0');
        return;
    }
    char digits[10] = { 0 };
    int d = 0;
    while (x != 0) {
        char c = x % 10;
        digits[d++] = c + '0';
        x = x / 10;
    }
    for (int i = d-1; i >= 0; i--)
        terminal_putchar(digits[i]);
}

void terminal_printf(const char *s, int x) {
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        if (s[i] == '%') {
            terminal_print_uint32(x);
        }
        else {
            terminal_putchar(s[i]);
        }
    }
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

void kernel_main(void)
{
    terminal_initialize();
    for (int i = 1; i <= 25; i++) {
        terminal_printf("Hello, kernel World %!\n", i);
    }
}
