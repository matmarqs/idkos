#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void term_initialize(unsigned long magic, unsigned long addr);
void term_putchar(char c);
void term_write(const char *data, size_t size);
void term_writestring(const char *data);

#endif /* _KERNEL_TTY_H */
