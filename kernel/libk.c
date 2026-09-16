#include <libk.h>
#include <vga.h>
#include <cursor.h>
#include <string.h>
#include <serial.h>

void screen_init() {
    serial_init(COM1_PORT);
    term_init();
    term_set_offset(cursor_get_offset());
}

/* putchar and print are independent (they don't call each other).
   I did this because print only needs to call cursor_set_offset at the end. */

int putchar(int ic) {
    term_putchar((char)ic);
    serial_putchar((char)ic);
    cursor_set_offset(term_get_offset());
    return ic;
}

int print(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        term_putchar(str[i]);
        serial_putchar(str[i]);
    }
    cursor_set_offset(term_get_offset());
    return len;
}
