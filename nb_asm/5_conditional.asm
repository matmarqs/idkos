    mov bx, 50

    cmp bx, 4
    jg less_40
    mov al, 'A'
    jmp done
less_40:
    cmp bx, 40
    jge last_else
    mov al, 'B'
    jmp done
last_else:
    mov al, 'C'

done:
    mov ah, 0x0e
    int 0x10

    jmp $

    times 510-($-$$) db 0
    dw 0xaa55
