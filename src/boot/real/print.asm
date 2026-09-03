[bits 16]

clear_screen:
    pusha

    mov ah, 0x6         ; ah = 0x6 = scroll window up function
    mov al, 0x0         ; scroll 0 lines = clear entire window
    mov bh, 0x7         ; default attribute (0x7 = light gray on black)
    mov cx, 0x0000      ; cx = top-left corner (row 0, col 0)
    mov dx, 0x184f      ; dx = bottom-right corner (row 24, col 79)
    int 0x10            ; call BIOS service

    mov ah, 0x2         ; ah = 0x2 (set cursor position function)
    mov bh, 0           ; bh = video page number (0)
    mov dx, 0x0000      ; dx = new coordinates (row 0, col 0)
    int 0x10

    popa
    ret

;; dx = 16-bit value to print in hexadecimal 0x0000
print_hex:
    pusha
    mov bx, HEX_OUT + 5
    mov al, 4

.loop:
    push dx         ;; save dx
    and dl, 0xf     ;; get hex digit
    cmp dl, 9
    jg .greater_than_9
    add dl, '0'     ;; '0' + 5 = '5'
    jmp .done_compare
.greater_than_9:
    add dl, 0x57    ;; 0x57 + 10 = 0x61 = 'a'
.done_compare:
    mov [bx], dl    ;; write to template string
    pop dx          ;; restore dx
    shr dx, 4       ;; shift 4 bits right
    dec bx
    dec al
    cmp al, 0
    jg .loop

    ;; print the template string now
    mov bx, HEX_OUT
    call print_string
    popa
    ret

HEX_OUT:
    db '0x0000', 0

;; bx = address of null-terminated string to print
print_string:
    pusha
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    call print_char
    inc bx
    jmp .loop
.done:
    popa
    ret

;; al = char to print
print_char:
    push ax
    mov ah, 0x0e
    int 0x10
    pop ax
    ret
