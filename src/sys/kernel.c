#include "sys/kernel.h"
#include "sys/idt.h"
#include "sys/io.h"
#include "sys/gdt.h"
#include "sys/config.h"
#include "sys/status.h"
#include "proc/tss.h"
#include "proc/task.h"
#include "proc/process.h"
#include "proc/isr80h.h"
#include "mem/heap.h"
#include "mem/kheap.h"
#include "mem/paging.h"
#include "libc/stdlib.h"
#include "fs/disk.h"
#include "fs/file.h"
#include "libc/stdio.h"
#include "dev/keyboard.h"

static struct paging_4gb_chunk* kernel_chunk = 0;
struct tss tss;
struct gdt gdt_real[DEXTER_TOTAL_GDT_SEGMENTS];

struct gdt_structured gdt_structured[DEXTER_TOTAL_GDT_SEGMENTS] = 
{
    {.base = 0x00, .limit = 0x00, .type = 0x00},                    // NULL segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0x9a},              // Kernel code segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0x92},              // Kernel data segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0xF8},              // User code segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0xF2},              // User data segment
    {.base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xE9}    // Task switch segment
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

void load_user_program_test()
{
    print("Starting process load.\n");

    struct process* process = 0;
    int r = process_load("0:/blank.bin", &process);

    if(r != DEXTER_ALL_OK)
    {
        panic("Failed to load blank.bin");
    }

    print("Calling task_run_first_task.\n");
    task_run_first_ever_task();
    print("Task running.\n");
}

// Enables memory paging using the enable_paging() function from paging.c
void start_paging() 
{
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(kernel_chunk);
    
    enable_paging();
}

// Load the global descriptor table for protected mode.
void setup_gdt()
{
    memset(gdt_real, 0x00, sizeof(gdt_real));

    gdt_structured_to_gdt(gdt_real, gdt_structured, DEXTER_TOTAL_GDT_SEGMENTS);
    gdt_load(gdt_real, sizeof(gdt_real));
}

// Load the task switching segment into the global descriptor table
void setup_tss()
{
    memset(&tss, 0x00, sizeof(tss));

    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    tss_load(0x28);
}

void pic_timer_callback(struct interrupt_frame* frame)
{
    print("Timer activated\n");
}

void panic(const char* msg)
{
    print("\n");
    print(msg);

    while(1);
}

void kernel_page()
{
    kernel_registers();
    paging_switch(kernel_chunk);
}

void kernel_init()
{
    // Initialize terminal for text mode
    terminal_initialize();
    
    cprint("THIS IS DEXTER!\n\n", 14);
    
    // Load global descriptor table
    setup_gdt();
#ifdef VERBOSE
    cprint("Global descriptor table loaded.\n", 15);
#endif

    // Initialize heap
    kheap_init();
#ifdef VERBOSE
    cprint("Kernel memory heap initialized.\n", 13);
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

    // Register interrupt 80 kernel commands
    isr80h_register_commands();
#ifdef VERBOSE
    print("Kernel commands registered.\n");
#endif

    // Initialize interrupt descriptor table
    idt_init();
#ifdef VERBOSE
    cprint("Interrupt descriptor table initialized.\n", 13);
#endif

    // Initialize keyboard
    keyboard_init();
#ifdef VERBOSE
    print("Keyboard initialized.\n");
#endif
//    idt_register_interrupt_callback(0x20, pic_timer_callback);

    // Setup the disk
    disk_search_and_init();
#ifdef VERBOSE
    cprint("Primary disk initialized.\n", 14);
#endif

    // Setup the task switching segment
    setup_tss();
#ifdef VERBOSE
    cprint("Task switching segment loaded.\n", 10);
#endif

    // Enable interrupts
     enable_interrupts();
// #ifdef VERBOSE
//     cprint("Interrupts enabled.\n\n", 12);
// #endif

}

void kernel_main() 
{
    kernel_init();

#ifdef DEBUG
    memory_allocation_test();
    cprint("Memory allocation test completed.\n", 12);
#endif

#ifdef DEBUG
    open_file("0:/hello.txt");
    open_file("0:/license.txt");

    load_user_program_test();
    print("load_user_program_test function exited.\n");
#endif

    while(1);
}
