/* reference: https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html */

#define ASM_FILE 1  /* multiboot2 uses this def */
#include "multiboot2.h"

#ifdef __ELF__
# define AOUT_KLUDGE 0
#else
# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif

    .section .multiboot
    .align 8

multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC
    .long 0     /* 0 for i386 */
    /* header length */
    .set HEADER_LENGTH, multiboot_header_end - multiboot_header
    .long HEADER_LENGTH
    /* checksum */
    .long -(MULTIBOOT2_HEADER_MAGIC + HEADER_LENGTH)

#ifndef __ELF__

address_tag_start:
    .short MULTIBOOT_HEADER_TAG_ADDRESS
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long address_tag_end - address_tag_start
    .long multiboot_header /* header_addr */
    .long _start  /* load_addr */
    .long _edata  /* load_end_addr */
    .long _end    /* bss_end_addr */
address_tag_end:

    .align 8

entry_address_tag_start:
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long entry_address_tag_end - entry_address_tag_start
    .long multiboot_entry  /* entry_addr */
entry_address_tag_end:

    .align 8

#endif /* __ELF__ */

framebuffer_tag_start:
    .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long framebuffer_tag_end - framebuffer_tag_start
    /* let GRUB choose width, height and depth by setting 0 */
    .long 0
    .long 0
    .long 0
framebuffer_tag_end:

    .align 8

    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8

    .align 8

multiboot_header_end:


    .section .bss
    .align 16
stack_bottom:
    .skip 0x4000
stack_top:


    .section .text

    .global _start
    .type _start, @function

_start:
    mov $stack_top, %esp

    /* Reset EFLAGS */
    pushl $0
    popf

    /* Push the pointer to the Multiboot information structure */
    pushl %ebx
    /* Push the magic value */
    pushl %eax

    /* Enter the C main function */
    call kernel_main

    /* Print the halt message */
    pushl $halt_message
    call term_writestring

    cli
1:  hlt
    jmp 1b

    /* set the size of the _start symbol to the current location '.'
       minus its start. this is useful when debugging or when you
       implement call tracing */
    .size _start, . - _start

halt_message:
    .asciz "Halted."
