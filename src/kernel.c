#include "kernel.h"
#include "idt.h"
#include "io.h"
#include "console.h"

void kernel_main() {
    terminal_initialize();
    
    print("THIS IS DEXTER!\n");
    
    idt_init();
    print("Interupt descriptor table initialized.\n");

    //outb(0x60, 0xff);
}
