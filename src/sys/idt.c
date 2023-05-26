#include "sys/idt.h"
#include "sys/config.h"
#include "sys/kernel.h"
#include "sys/io.h"
#include "mem/memory.h"
#include "proc/task.h"
#include "lib/console.h"

struct idt_desc idt_descriptors[DEXTER_TOTAL_INTERUPTS];
struct idtr_desc idtr_descriptor;

static ISR80H_COMMAND isr80h_commands[DEXTER_MAX_ISR80H_COMMANDS];

extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();
extern void isr80h_wrapper();

void int21h_handler() 
{
    print("Keyboard pressed!\n");

    outb(0x20, 0x20);
}

void* isr80h_handle_command(int command, struct interrupt_frame* frame)
{
    void* r = 0;

    if(command <= 0 || command >= DEXTER_MAX_ISR80H_COMMANDS)
    {
        return 0;
    }

    ISR80H_COMMAND command_func = isr80h_commands[command];

    if(!command_func)
    {
        return 0;
    }

    r = command_func(frame);

    return r;
}

void* isr80h_handler(int command, struct interrupt_frame* frame)
{
    void* r = 0;
    kernel_page();

    task_current_save_state(frame);
    r = isr80h_handle_command(command, frame);

    task_page();

    return r;
}

void no_interrupt_handler() 
{
    outb(0x20, 0x20);
}

void idt_zero() 
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void* address) 
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];

    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = (uint32_t) address >> 16;
}

void isr80h_register_command(int command_id, ISR80H_COMMAND command)
{
    if(command_id <= 0 || command_id >= DEXTER_MAX_ISR80H_COMMANDS)
    {
        panic("isr80h_register_command(): Command is out of bounds.\n");
    }

    if(isr80h_commands[command_id])
    {
        panic("isr80h_register_command(): Cannot register command because it will override an existing command.\n");
    }

    isr80h_commands[command_id] = command;
}

void idt_init() 
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));

    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    for (int i = 0; i < DEXTER_TOTAL_INTERUPTS; i++)
    {
        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, int21h);
    idt_set(0x80, isr80h_wrapper);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}