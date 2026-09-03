; Low-level functions that aren't possible to be written in C
[bits 32]

global port_byte_in
global port_byte_out
global port_word_in
global port_word_out

; unsigned char port_byte_in(unsigned short port);
port_byte_in:
    push dx
    mov dx, word [esp+8]
    in al, dx
    pop dx
    ret

; void port_byte_out(unsigned short port, unsigned char data);
port_byte_out:
    push ebp
    mov ebp, esp
    pusha
    mov al, byte [ebp+12]
    mov dx, word [ebp+8]
    out dx, al
    popa
    leave
    ret

; unsigned short port_word_in(unsigned short port);
port_word_in:
    push dx
    mov dx, word [esp+8]
    in ax, dx
    pop dx
    ret

; void port_word_out(unsigned short port, unsigned short data);
port_word_out:
    push ebp
    mov ebp, esp
    pusha
    mov ax, word [ebp+12]
    mov dx, word [ebp+8]
    out dx, ax
    popa
    leave
    ret
