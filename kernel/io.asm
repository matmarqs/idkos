[bits 32]

; void outb(uint16_t port, uint8_t byte);
global outb
outb:
    mov al, [esp+8]     ; byte to be sent into 'al'
    mov dx, [esp+4]     ; address of IO port into 'dx'
    out dx, al
    ret

; uint8_t inb(uint16_t port);
global inb
inb:
    mov dx, [esp+4]     ; address of I/O port into 'dx'
    in al, dx           ; read a byte from I/O port and store into 'al'
    ret
