#ifndef _SERIAL_H
#define _SERIAL_H

#define COM1_PORT 0x3F8

int serial_init(unsigned short com);
void serial_putchar(char c);

#endif /* _SERIAL_H */
