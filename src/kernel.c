#include "kernel.h"
#include "idt.h"
#include "io.h"
#include "console.h"

size_t strlen(const char* str) {
    size_t len = 0;

    while(str[len])
    {
        len++;
    }

    return len;
}

void kernel_main() {
    terminal_initialize();
    
    print("THIS IS DEXTER!\n");
    
    idt_init();
    print("Interupt descriptor table initialized.\n");

    //outb(0x60, 0xff);
}