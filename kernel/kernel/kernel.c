#include <kernel/tty.h>
#include <kernel/io.h>

#include <stdint.h>

static char vendor_string[13];

void terminal_print_uint32(uint32_t x) {
    if (x == 0) {
        term_putchar('0');
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
        term_putchar(digits[i]);
}

void kernel_main(unsigned long magic, unsigned long addr)
{
    term_initialize(magic, addr);
    term_writestring("Hello world\n");

    if (get_cpu_vendor_string(vendor_string)) {
        term_writestring("Your CPU is ");
        term_writestring(vendor_string);
        term_putchar('\n');
    }

    while (1) {
        terminal_print_uint32(poll_keyboard());
        term_putchar('\n');
    }

    return;
}
