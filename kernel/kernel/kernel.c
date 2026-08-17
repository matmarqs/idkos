#include <kernel/tty.h>
#include <kernel/io.h>

static char vendor_string[13];

void kernel_main(unsigned long magic, unsigned long addr)
{
    term_initialize(magic, addr);
    term_writestring("Hello world\n");

    if (get_cpu_vendor_string(vendor_string)) {
        term_writestring("Your CPU is ");
        term_writestring(vendor_string);
        term_putchar('\n');
    }

    return;
}
