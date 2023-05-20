#include "../sys/kernel.h"
#include "../sys/idt.h"
#include "../sys/io.h"
#include "../sys/gdt.h"
#include "../sys/config.h"
#include "../mem/heap.h"
#include "../mem/kheap.h"
#include "../mem/paging.h"
#include "../mem/memory.h"
#include "../fs/disk.h"
#include "../fs/file.h"
#include "../lib/console.h"

static struct paging_4gb_chunk* kernel_chunk = 0;
struct gdt gdt_real[DEXTER_TOTAL_GDT_SEGMENTS];

struct gdt_structured gdt_structured[DEXTER_TOTAL_GDT_SEGMENTS] = 
{
    {.base = 0x00, .limit = 0x00, .type = 0x00},            // NULL segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0x9a},      // Code segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0x92}       // Data segment
};

// Test code for memory allocation uusing kmalloc and kfree.
int memory_allocation_test() 
{
    void* ptr = kmalloc(50);
    void* ptr2 = kmalloc(5000);

    if(ptr == ptr2) ;

    kfree(ptr);
    kfree(ptr2);

    return 0;
}

// Function to test file open, read, seek, stat and close operations.
void open_file(const char* filename)
{
    int fd = fopen(filename, "r");

    if(fd)
    {
        cprint(filename, 10);
        cprint(" file opened successfully.\n", 10);
    }
    else
    {
        cprint(filename, 10);
        cprint(" file failed to open.\n", 10);

        goto out;
    }

    char buf[14];
    
    fseek(fd, 2, SEEK_SET);    
    fread(buf, 11, 1, fd);

    buf[13] = 0x00;

    print(buf);

    struct file_stat s;
    fstat(fd, &s);

    fclose(fd);

    print("\nFile closed.\n");
out:
    return;
}

// Enables memory paging using the enable_paging() function from paging.c
int start_paging() 
{
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    
    enable_paging();

    return 0;
}

// Load the global descriptor table for protected mode.
int setup_gdt()
{
    memset(gdt_real, 0x00, sizeof(gdt_real));

    gdt_structured_to_gdt(gdt_real, gdt_structured, DEXTER_TOTAL_GDT_SEGMENTS);
    gdt_load(gdt_real, sizeof(gdt_real));

    return 0;
}

void panic(const char* msg)
{
    print("\n");
    print(msg);

    while(1);
}

void kernel_main() 
{
    // Initialize terminal for text mode
    terminal_initialize();
    
    cprint("THIS IS DEXTER!\n\n", 14);
    
    // Load global descriptor table
    setup_gdt();
#ifdef VERBOSE
    cprint("Global descriptor table loaded.", 15);
#endif

    // Initialize heap
    kheap_init();
#ifdef VERBOSE
    cprint("Kernel memory heap initialized.\n", 13);
#endif
#ifdef DEBUG
    memory_allocation_test();
    cprint("Memory allocation test completed.\n", 12);
#endif

    // Inititalize file systems
    fs_init();
#ifdef VERBOSE
    cprint("File systems initialized.\n", 11);
#endif

    // Setup and enable memory paging
    start_paging();
#ifdef VERBOSE
    cprint("Paging enabled.\n", 10);
#endif

    // Setup the disk
    disk_search_and_init();
#ifdef VERBOSE
    cprint("Primary disk initialized.\n", 14);
#endif

    // Initialize interrupt descriptor table
    idt_init();
#ifdef VERBOSE
    cprint("Interrupt descriptor table initialized.\n", 13);
#endif
    enable_interrupts();
#ifdef VERBOSE
    cprint("Interrupts enabled.\n\n", 12);
#endif

#ifdef DEBUG
    open_file("0:/hello.txt");
    open_file("0:/license.txt");
#endif

    while(1);
}
