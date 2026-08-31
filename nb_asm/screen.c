#include "screen.h"

int get_cursor() {
  // The device uses its control register as an index
  // to select its internal registers, of which we are
  // interested in:
  //   reg 14: which is the high byte of the cursor's offset
  //   reg 15: which is the low byte of the cursor's offset
  // Once the internal register has been selected, we may read or
  // write a byte on the data register.
  port_byte_out(REG_SCREEN_CTRL, 14);
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);
  // we multiply it by 2 because VGA is 16 bits each cell.
  return offset * 2;
}

/* Print a char on the screen at col, row, or at a cursor position */
void print_char(char character, int col, int row, char attribute_byte) {
  unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;

  if (!attribute_byte) {
    attribute_byte = WHITE_ON_BLACK;
  }

  int offset;
  if (col >= 0 && row >= 0) {
    offset = get_screen_offset(col, row);
  } else {
    offset = get_cursor();
  }

  if (character == '\n') {
    int rows = offset / (2 * MAX_COLS);
    offset = get_screen_offset(79, rows);
  } else {
    vidmem[offset] = character;
    vidmem[offset + 1] = attribute_byte;
  }

  offset += 2;
  offset = handle_scrolling(offset);
  set_cursor(offset);
}
