#include "kernel.h"
#include "idt.h"
#include "io.h"
#include "console.h"

void kernel_main() {
    terminal_initialize();
    
    print("THIS IS DEXTER!\n");
    
    idt_init();
    print("Interrupt descriptor table initialized.\n");

    enable_interrupts();
    print("Interrupts enabled.\n");

    while(1);
}
