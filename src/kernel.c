#include "kernel.h"
#include "idt.h"
#include "io.h"
#include "console.h"

void kernel_main() {
    terminal_initialize();
    
    cprint("THIS IS DEXTER!\n\n", 14);
    
    idt_init();
    print("Interrupt descriptor table initialized.\n");

    enable_interrupts();
    print("Interrupts enabled.\n");

    while(1);
}
