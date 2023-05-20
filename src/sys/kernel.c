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

    kfree(ptr);
    kfree(ptr2);

    return 0;
}

int start_paging() 
{
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    
    enable_paging();

    return 0;
}

void panic(const char* msg)
{
    print("\n");
    print(msg);

    while(1);
}

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

void kernel_main() 
{
    // Initialize terminal for text mode
    terminal_initialize();
    
    cprint("THIS IS DEXTER!\n\n", 14);
    
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
