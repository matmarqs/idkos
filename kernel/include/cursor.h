#ifndef _CURSOR_H
#define _CURSOR_H

#include <stdint.h>
#include <stddef.h>

void cursor_set_offset(uint16_t offset);
size_t cursor_get_offset();

#endif  /* _CURSOR_H */
