#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
    term_putchar((char)ic);
#else
    // TODO: implement stdio and the `write` syscall.
#endif
    return ic;
}
