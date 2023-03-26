#include "kernel.h"
#include "idt.h"
#include "io.h"
#include "heap.h"
#include "kheap.h"
#include "console.h"

void kernel_main() {
    // Initialize terminal for text mode
    terminal_initialize();
    
    cprint("THIS IS DEXTER!\n\n", 14);
    
    // Initialize heap
    kheap_init();
    cprint("Kernel memory heap initialized.\n", 13);

    void* ptr = kmalloc(50);
    void* ptr2 = kmalloc(5000);
    cprint("Memory allocation test completed.\n", 12);
    if(ptr == ptr2) ;

    // Initialize interrupt descriptor table
    idt_init();
    cprint("Interrupt descriptor table initialized.\n", 11);

    enable_interrupts();
    cprint("Interrupts enabled.\n", 10);

    while(1);
}

