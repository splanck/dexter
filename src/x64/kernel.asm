[BITS 64]

global _start
extern kernel_main

_start:
    mov rsp, 0x00200000
    call kernel_main
.hang:
    hlt
    jmp .hang
