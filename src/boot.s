ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop
    
times 33 db 0
start:
    jmp 0:step2 ; Set start of code segment
    
step2:
    cli ; Clear interupts
    mov ax, 0x00
    mov ds, ax ; Set data segment to 0x7c0
    mov es, ax
    mov ss, ax ; Set stack segment to zero
    mov sp, 0x7c00
    sti ; Enable interupts
    
    mov si, message
    call print
    
    jmp load_protected
    
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

load_protected: ; Switch into 32bit mode
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

gdt_code: ; Code segment
    dw 0xffff
    dw 0
    db 0
    db 0x9a
    db 11001111b
    db 0

gdt_data: ; Data segment
    dw 0xffff
    dw 0
    db 0
    db 0x92
    db 11001111b
    db 0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    ; Initialize 32bit registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al
    
    jmp $
    
; Add the boot signature
times 510-($ - $$) db 0
dw 0xAA55