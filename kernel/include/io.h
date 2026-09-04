#ifndef _IO_H
#define _IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t byte);
uint8_t inb(uint16_t port);

#endif /* _IO_H */
