[bits 16]

; load DH sectors to ES:BX from drive DL
disk_load:
    pusha
    push dx

    mov ah, 0x02    ; BIOS read sector function
    mov al, dh      ; read DH sectors
    mov ch, 0x00    ; cylinder 0
    mov dh, 0x00    ; head 0
    mov cl, 0x02    ; second sector

    int 0x13

    jc disk_error   ; jump if error (carry flag set)

    pop dx
    cmp dh, al      ; if AL (sectors read) != DH (sectors expected)
    jne disk_error

    popa
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

DISK_ERROR_MSG:
    db "Disk read error!", 0xd, 0xa, 0
