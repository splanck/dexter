#include "proc/process.h"
#include "proc/task.h"
#include "sys/config.h"
#include "sys/status.h"
#include "sys/kernel.h"
#include "mem/memory.h"
#include "mem/kheap.h"
#include "mem/paging.h"
#include "fs/file.h"
#include "lib/string.h"

struct process* current_process = 0;
static struct process* processes[DEXTER_MAX_PROCESSES] = {};

static void process_init(struct process* process)
{
    memset(process, 0, sizeof(struct process));
}

struct process* process_current()
{
    return current_process;
}

int process_load_binary(const char* filename, struct process* process)
{
    int r = 0;

    int fd = fopen(filename, "r");

    if(!fd)
    {
        r = -EIO;
        goto out;
    }

    struct file_stat stat;
    r = fstat(fd, &stat);

    if(r != DEXTER_ALL_OK)
    {
        goto out;
    }

    void* program_data_ptr = kzalloc(stat.filesize);

    if(!program_data_ptr)
    {
        r = -ENOMEM;
        goto out;
    }

    r = fread(program_data_ptr, stat.filesize, 1, fd);

    if(r != DEXTER_ALL_OK)
    {
        goto out;
    }

    process->ptr = program_data_ptr;
    process->size = stat.filesize;
out:
    fclose(fd);

    return r;
}

int process_load_data(const char* filename, struct process* process)
{
    int r = 0;
    r = process_load_binary(filename, process);
    return r;
}

int process_load_for_slot(const char* filename, struct process** process, int process_slot)
{
    int r = 0;

    if(process_get(process_slot) != 0)
    {
        r = -EISTKN;
        goto out;
    }

    struct process* _process = kzalloc(sizeof(struct process));

    if(!_process)
    {
        r = -ENOMEM;
        goto out;
    }

    process_init(_process);

    // Read the program file into memory.
    r = process_load_data(filename, _process);

    if(r < 0)
    {
        goto out;
    }

    // Allocate memory for process stack.
    void* program_stack_ptr = kzalloc(DEXTER_USER_PROGRAM_STACK_SIZE);

    if(!program_stack_ptr)
    {
        r = -ENOMEM;
        goto out;
    }

    // Populate process structure with filename, stack, and process id.
    strncpy(_process->filename, filename, sizeof(_process->filename));
    _process->stack = program_stack_ptr;
    _process->id = process_slot;

    // Create task
    struct task* task = task_new(_process);

    if(ERROR_I(task) == 0)
    {
        r = ERROR_I(task);
    }

    _process->task = task;

    r = process_map_memory(_process);

    if (r < 0)
    {
        goto out;
    }

    *process = _process;

    // Add the new process to the processes array.
    processes[process_slot] = *process;
out: 
    if(ISERR(r))
    {
        if(_process && _process->task)
        {
            task_free(_process->task);
        }
    }

    return r;
}

int process_map_binary(struct process* process)
{
    int r = 0;
    paging_map_to(process->task->page_directory->directory_entry, (void*) DEXTER_PROGRAM_VIRTUAL_ADDRESS,
        process->ptr, paging_align_address(process->ptr + process->size), 
        PAGING_IS_PRESENT | PAGING_IS_WRITEABLE | PAGING_ACCESS_FROM_ALL);
    return r;
}

int process_map_memory(struct process* process)
{
    int r = 0;
    r = process_map_binary(process);
    return r;
}

int process_get(int process_id)
{
    int r = 0;

    if(process_id < 0 || process_id > DEXTER_MAX_PROCESSES)
    {
        r = -EINVARG;
        goto out;
    }

    r = processes[process_id];
out:
    return r;
}

