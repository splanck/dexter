#ifndef TASK_H
#define TASK_H

#include "sys/config.h"
#include "mem/paging.h"

struct interrupt_frame;
struct process;

struct registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
};

struct task
{
    /**
     * The page directory of the task
     */
    struct paging_4gb_chunk* page_directory;

    // The registers of the task when the task is not running
    struct registers registers;

    // The process of the task
    struct process* process;

    // The next task in the linked list
    struct task* next;

    // Previous task in the linked list
    struct task* prev;
};

struct task* task_new(struct process* process);
struct task* task_current();
struct task* task_get_next();
int task_free(struct task* task);
int task_switch(struct task* task);
int task_page();
void task_current_save_state(struct interrupt_frame* frame);
void task_save_state(struct task* task, struct interrupt_frame* frame);
void task_run_first_ever_task();
void task_return(struct registers* regs);
void restore_general_purpose_registers(struct registers* regs);
void user_registers();

#endif