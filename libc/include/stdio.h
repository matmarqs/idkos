#ifndef _STDIO_H
#define _STDIO_H

#include <sys/cdefs.h>

#define EOF (-1)

int printf(const char *__restrict format, ...);
int putchar(int i);
int puts(const char *str);

#endif /* _STDIO_H */
