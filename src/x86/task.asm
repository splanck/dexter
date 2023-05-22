[BITS 32]
section .asm

global task_return
global restore_general_purpose_registers

task_return:
    mov ebp, esp

    mov ebx, [ebp+4]
    push dword [ebx+44]
    push dword [ebx+40]

    pushf
    pop eax
    or eax, 0x200
    push eax

    push dword [ebx+32]
    push dword [ebx+28]

    mov ax, [ebx+44]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword [ebx+4]
    call restore_general_purpose_registers
    add esp, 4

    iretd

restore_general_purpose_registers:
    push ebp
    
    mov ebp, esp
    mov ebx, [ebp+8]
    mov edi, ebx
    mov esi, [ebp+4]
    mov ebp, [ebp+8]
    mov edx, [ebp+16]
    mov ecx, [ebp+20]
    mov eax, [ebp+24]
    mov ebx, [ebp+12]
    
    pop ebp
    
    ret
