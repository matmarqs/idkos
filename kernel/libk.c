#include <libk.h>
#include <vga.h>
#include <cursor.h>

void screen_init() {
    term_init();
    term_set_offset(cursor_get_offset());
}

int putchar(int ic) {
    term_putchar((char) ic);
    return ic;
}
