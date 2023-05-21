#ifndef TASK_H
#define TASK_H

#include "sys/config.h"
#include "mem/paging.h"
#include <stdint.h>

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
    struct paging_4gb_chunk* page_directory;    // Task page directory
    struct registers registers;                // Registers when task is not running
    struct task* next;                          // Next task in the linked list
    struct task* previous;                      // Previous task in the linked list
};

int task_init(struct task* task);
int task_free(struct task* task);
struct task* task_new();
struct task* task_current();
struct task* task_get_next();

#endif