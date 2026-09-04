[org 0x7c00]
KERNEL_OFFSET equ 0x1000    ; memory offset which we will load our kernel

    mov [BOOT_DRIVE], dl    ; BIOS stores out boot drive in DL, let's save it

    mov bp, 0x9000          ; set the stack safely at 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE   ; announce we are in 16-bit real mode
    call print_string

    call load_kernel        ; load our kernel

    call switch_to_pm       ; switch to protected mode, from which we will not return

    jmp $                   ; Hang

%include "real/print.asm"
%include "real/disk_load.asm"
%include "protected/gdt.asm"
%include "protected/switch_to_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL     ; print message saying we are loading the kernel
    call print_string

    mov bx, KERNEL_OFFSET       ; set up parameters for our disk_load routine, so
    mov dh, 15                  ; that we load the first 15 sectors (excluding
    mov dl, [BOOT_DRIVE]        ; the boot sector) from the boot disk (i.e. our
    call disk_load              ; kernel code) to address KERNEL_OFFSET

    ret

[bits 32]
;; This is where we arrive after switching to and initializing protected mode.
BEGIN_PM:
    call KERNEL_OFFSET          ; Now jump to the address of our loaded kernel code

    jmp $                       ; Hang

; Global variables
BOOT_DRIVE:     	db 0
MSG_REAL_MODE:  	db "Started in 16-bit Real Mode", 0xd, 0xa, 0 ; "0xd, 0xa" is the CR-LF needed for "\n"
MSG_LOAD_KERNEL:    db "Loading kernel into memory.", 0xd, 0xa, 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
