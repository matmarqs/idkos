    /* there is something called the Multiboot Standard. */
    /* it describes an easy interface between the bootloader and the OS kernel */
    /* it works by putting a few magic values (the multiboot header), which is searched by the bootloader (GRUB) */
    /* GRUB will know how to load us, given these magic values. */

    /* multiboot header */
    .set ALIGN,     1<<0             /* align loaded modules on page boundaries */
    .set MEMINFO,   1<<1             /* provide memory map */
    .set FLAGS,     ALIGN | MEMINFO  /* Multiboot 'flag' field */
    .set MAGIC,     0x1BADB002       /* 'magic number' lets bootloader find the header */
    .set CHECKSUM,  -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */


    /* multiboot header that marks the program as a kernel. these are magic values that */
    /* are documented in the multiboot standard. the bootloader will search for this signature */
    /* in the first 8kb of the kernel file, aligned at a 32-bit boundary. the signature is in its */
    /* own section so the header can be forced to be within the first 8kb of the kernel file */

    .section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

    /* it is up to the kernel to provide a stack */
    /* the stack on x86 must be 16-byte aligned according to the System V ABI standard. */
    /* the compiler assumes the stack is properly aligned and failure to align the stack */
    /* will result in undefined behavior. */
    .section .bss
    .align 16
stack_bottom:
    .skip 16834  /* 16 KiB */
stack_top:

    /* the linker script specifies _start as the entry point to the kernel and the bootloader will */
    /* jump to this position once the kernel has been loaded. it doesn't make sense to return from */
    /* this function as the bootloader is gone */
    .section .text
    .global _start
    .type _start, @function
_start:
    /* the bootloader has loaded us into 32-bit protected mode on a x86 machine. interrupts are disabled. */
    /* paging is disabled. the processor state is as defined in the multiboot standard. the kernel has full */
    /* control of the CPU. the kernel can only make use of hardware features and any code it provides as */
    /* part of itself */

    /* to set up a stack, we set the esp register to point to the top of the stack. */
    mov $stack_top, %esp

    /* this is a good place to initialize crucial processor state before the high-level kernel is entered. */

    /* enter the high-level kernel. the ABI requires the stack is 16-byte aligned at the time of the call */
    /* instruction. */
    call kernel_main

    /* if the system has nothing more to do, put the computer into an infinite loop. to do that: */
    /* 1) disable interrupts with cli */
    /* 2) wait for the next interrupt to arrive with hlt (halt instruction). */
    /*    since they are disabled, this will lock up the computer. */
    /* 3) jump to the hlt instruction if it ever wakes up due to a */
    /*    non-maskable interrupt occuring or due to system management mode. */
    cli
1:  hlt
    jmp 1b

    /* set the size of the _start symbol to the current location '.' minus its start */
    /* this is useful when debugging or when you implement call tracing */
    .size _start, . - _start
