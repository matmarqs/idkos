#include <libk.h>
#include <vga.h>
#include <cursor.h>
#include <string.h>

void screen_init() {
    term_init();
    term_set_offset(cursor_get_offset());
}

int putchar(int ic) {
    term_putchar((char)ic);
    cursor_set_offset(term_get_offset());
    return ic;
}

int print(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        term_putchar(str[i]);
    }
    cursor_set_offset(term_get_offset());
    return len;
}
