#include "../sys/kernel.h"
#include "../sys/idt.h"
#include "../sys/io.h"
#include "../mem/heap.h"
#include "../mem/kheap.h"
#include "../mem/paging.h"
#include "../fs/disk.h"
#include "../fs/file.h"
#include "../lib/console.h"

static struct paging_4gb_chunk* kernel_chunk = 0;

int memory_allocation_test() 
{
    void* ptr = kmalloc(50);
    void* ptr2 = kmalloc(5000);

    if(ptr == ptr2) ;

    return 0;
}

int start_paging() 
{
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    
    enable_paging();

    return 0;
}

void kernel_panic() 
{
    print("\nOops. Something just went terribly wrong. PANIC!\n");
    while(1);
}

void kernel_main() 
{
    // Initialize terminal for text mode
    terminal_initialize();
    
    cprint("THIS IS DEXTER!\n\n", 14);
    
    // Initialize heap
    kheap_init();
    cprint("Kernel memory heap initialized.\n", 13);

    memory_allocation_test();
    cprint("Memory allocation test completed.\n", 12);

    // Inititalize file systems
    fs_init();
    cprint("File systems initialized.\n", 11);
    
    // Setup and enable memory paging
    start_paging();
    cprint("Paging enabled.\n", 10);

    // Setup the disk
    disk_search_and_init();
    cprint("Primary disk initialized.\n", 14);

    // Initialize interrupt descriptor table
    idt_init();
    cprint("Interrupt descriptor table initialized.\n", 13);

    enable_interrupts();
    cprint("Interrupts enabled.\n", 12);

    char buf[512];
    disk_read_sectors(0, 1, buf);

    update_cursor(15,15);
    //print("Hello");

    while(1);
}
