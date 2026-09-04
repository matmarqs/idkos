#include <libk.h>
#include <vga.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

static size_t term_row;
static size_t term_col;
static uint8_t term_color;
static uint16_t *term_buffer = (uint16_t *) VGA_MEMORY;

void term_init(void)
{
    term_row = 0;
    term_col = 0;
    term_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void term_clear(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            term_buffer[index] = vga_entry(' ', term_color);
        }
    }
}

void term_setcolor(enum vga_color fg, enum vga_color bg)
{
    term_color = vga_entry_color(fg, bg);
}

size_t term_get_offset() {
    return term_row * VGA_WIDTH + term_col;
}

void term_set_offset(size_t offset) {
    term_row = offset / VGA_WIDTH;
    term_col = offset % VGA_WIDTH;
}

void term_putchar(char c) {
    if (c == '\n') {
        term_col = 0;
        if (++term_row == VGA_HEIGHT)
            term_row = 0;
        return;
    }
    term_buffer[term_row * VGA_WIDTH + term_col] = vga_entry(c, term_color);
    if (++term_col == VGA_WIDTH) {
        term_col = 0;
        if (++term_row == VGA_HEIGHT)
            term_row = 0;
    }
}
