#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H

#include <stddef.h>

int get_cpu_vendor_string(char vendor_string[13]);
int poll_keyboard(void);

#endif /* _KERNEL_IO_H */
