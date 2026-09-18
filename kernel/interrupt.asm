[bits 32]
[extern interrupt_handler] ; this one we write in C

; When an interrupt occurs, the CPU will push some information about the interrupt
; onto the stack, then look up the appropriate interrupt handler in the IDT and jump to it.
; The stack at the time of the interrupt will look like the following:
;     [esp + 12] eflags
;     [esp + 8]  cs
;     [esp + 4]  eip
;     [esp]      error code?  ; some interrupts put it, otherwise we put 0
; The specific CPU interrupts that put an error code on to the stack are:
; 8, 10, 11, 12, 13, 14 and 17.
; The error code can be used by the interrupt handler to get more information on what happened
; Also, note that the interrupt number is not pushed onto the stack, we push ourselves too!

%macro no_error_code_interrupt_handler %1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0        ; push 0 as error code, because it was not pushed automatically
    push dword %1       ; push the interrupt number
    jmp common_interrupt_handler ; jump to the common handler
%endmacro

%macro error_code_interrupt_handler %1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1       ; push the interrupt number
    jmp common_interrupt_handler ; jump to the common handler
%endmacro

; this one does not need to be global
common_interrupt_handler: ; common parts of the generic interrupt handler
    ; save the registers in reverse, see include/interrupt.h for the struct
    push esp
    push edi
    push esi
    push ebp
    push edx
    push ecx
    push ebx
    push eax

    call interrupt_handler ; args: (cpu_state cpu, uint32_t intr_num, intr_state intr)

    ; restore registers
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop ebp
    pop esi
    pop edi
    pop esp

    ; restore the esp
    add esp, 8          ; restore the error_code and intr_num (8 bytes)

    ; return to the code that got interrupted
    iret

; apply the macros correctly for interrupts 8, 10, 11, 12, 13, 14 and 17.
no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
error_code_interrupt_handler    8   ; error code is put automatically
no_error_code_interrupt_handler 9
error_code_interrupt_handler    10  ; error code is put automatically
error_code_interrupt_handler    11  ; error code is put automatically
error_code_interrupt_handler    12  ; error code is put automatically
error_code_interrupt_handler    13  ; error code is put automatically
error_code_interrupt_handler    14  ; error code is put automatically
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16
no_error_code_interrupt_handler 17
no_error_code_interrupt_handler 18
no_error_code_interrupt_handler 19
no_error_code_interrupt_handler 20
no_error_code_interrupt_handler 21
no_error_code_interrupt_handler 22
no_error_code_interrupt_handler 23
no_error_code_interrupt_handler 24
no_error_code_interrupt_handler 25
no_error_code_interrupt_handler 26
no_error_code_interrupt_handler 27
no_error_code_interrupt_handler 28
no_error_code_interrupt_handler 29
error_code_interrupt_handler    30  ; error code is put automatically
no_error_code_interrupt_handler 31
no_error_code_interrupt_handler 32

global load_idt
; void load_idt(struct idt *idt_addr);
; Loads the interrupt descriptor table (IDT)
load_idt:
    mov eax, [esp+4]    ; load idt_addr into register eax
    lidt eax            ; load the IDR
    ret                 ; return
