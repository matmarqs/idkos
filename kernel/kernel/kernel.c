#include <kernel/tty.h>

void kernel_main(unsigned long magic, unsigned long addr)
{
    term_initialize(magic, addr);
    term_writestring("Hello world\r\n");

    return;
}
