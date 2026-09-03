[org 0x7c00]

     mov [BOOT_DRIVE], dl   ; BIOS stores out boot drive in DL, let's save it

     mov bp, 0x8000     ; set the stack safely at 0x8000
     mov sp, bp

     mov bx, 0x9000     ; Load 5 sectors to 0x0000(ES):0x9000(BX)
     mov dh, 5          ; from the boot disk
     mov dl, [BOOT_DRIVE]
     call disk_load

     mov dx, [0x9000]   ; Print out the first loaded word, which
     call print_hex     ; we expect to be 0xdada, stored at 0x9000

     mov dx, [0x9000 + 512] ; Also, print the first word from the
     call print_hex         ; 2nd loaded sector: should be 0xface

     jmp $

%include "print_string.asm"
%include "disk_load.asm"

BOOT_DRIVE:
    db 0

times 510-($-$$) db 0
dw 0xaa55

;; this is after the 512-byte that BIOS loads
times 256 dw 0xdada
times 256 dw 0xface

times 3072-($-$$) db 0
