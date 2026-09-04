#include <cursor.h>
#include <io.h>

/* I/O ports */
#define FB_COMMAND_PORT  0x3d4
#define FB_DATA_PORT     0x3d5

/* I/O port commands */
#define FB_HIGH_BYTE_COMMAND  14
#define FB_LOW_BYTE_COMMAND   15

void cursor_set_offset(uint16_t offset) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, (offset >> 8) & 0x00ff);
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, offset & 0x00ff);
}

size_t cursor_get_offset() {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    int offset = inb(FB_DATA_PORT) << 8; /* high byte */
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    offset += inb(FB_DATA_PORT); /* low byte */
    return offset;  /* VGA offset, where each entry is 16-bits */
}
