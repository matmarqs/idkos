#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>

#include <stddef.h>

int memcmp(const void *ptr1, const void *ptr2, size_t n);
void *memcpy(void *__restrict dst, const void *__restrict src, size_t);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *ptr, int value, size_t n);
size_t strlen(const char *str);

#endif /* _STRING_H */
