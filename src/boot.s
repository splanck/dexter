ORG 0
BITS 16
_start:
    jmp short start
    nop
    
times 33 db 0
start:
    jmp 0x7c0:step2 ; Set start of code segment
    
step2:
    cli ; Clear interupts
    mov ax, 0x7c0
    mov ds, ax ; Set data segment to 0x7c0
    mov es, ax
    mov ax, 0x00    
    mov ss, ax ; Set stack segment to zero
    mov sp, 0x7c00
    sti ; Enable interupts
    mov si, message
    call print
    
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

message: db 'THIS IS DEXTER!', 0

; Add the boot signature
times 510-($ - $$) db 0
dw 0xAA55