#ifndef PROCESS_H
#define PROCESS_h

#include <stdint.h>
#include "sys/config.h"
#include "proc/task.h"

struct process
{
    // Process id
    uint16_t id;

    // Executable filename
    char filename[DEXTER_MAX_PATH];

    // Task for the current process
    struct task* task;

    // Memory allocations for process
    void* allocations[DEXTER_MAX_PROGRAM_ALLOCATIONS];

    // Pointer to process memory
    void* ptr;

    // Pointer to stack
    void* stack;

    // Size of ptr
    uint32_t size;
};

int process_load_for_slot(const char* filename, struct process** process, int process_slot);
struct process* process_current();
int process_get(int index);

#endif