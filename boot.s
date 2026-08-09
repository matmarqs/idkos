/* reference: https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html */


#define ASM_FILE 1  /* multiboot2 uses this def */
#include <multiboot2.h>

/* C symbol format. HAVE_ASM_USCORE is defined by configure */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)     _ ## sym
#else
# define EXT_C(sym)     sym
#endif

#define STACK_SIZE 0x4000

#ifdef __ELF__
# define AOUT_KLUDGE 0
#else
# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif
    
    .text

    .globl start, _start

start:  
_start: 
    jmp multiboot_entry

    .align 8

multiboot_header:   
    .long MULTIBOOT2_HEADER_MAGIC
    .long GRUB_MULTIBOOT_ARCHITECTURE_I386
    /* header length */
    .long multiboot_header_end - multiboot_header
    /* checksum */
    .long -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))

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

entry_address_tag_start:    
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long entry_address_tag_end - entry_address_tag_start
    .long multiboot_entry  /* entry_addr */
entry_address_tag_end:  

#endif /* __ELF__ */

framebuffer_tag_start:  
    .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long framebuffer_tag_end - framebuffer_tag_start
    .long 1024
    .long 768
    .long 32
framebuffer_tag_end:    

    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8

multiboot_header_end:   

multiboot_entry:    

    mov $(stack + STACK_SIZE), %esp

    /* Reset EFLAGS */
    pushl $0
    popf

    /* Push the pointer to the Multiboot information structure */
    pushl %ebx
    /* Push the magic value */
    pushl %eax
    
    /* Enter the C main function */
    call EXT_C(kernel_main)

    /* Print the halt message */
    pushl $halt_message
    call EXT_C(printf)

    cli
1:  hlt
    jmp 1b

    /* set the size of the _start symbol to the current location '.'
       minus its start. this is useful when debugging or when you
       implement call tracing */
    .size _start, . - _start

halt_message:   
    .asciz "Halted."
    
    /* Our stack area */
    .comm stack, STACK_SIZE
