#include "proc/process.h"
#include "proc/task.h"
#include "sys/config.h"
#include "sys/status.h"
#include "mem/memory.h"
#include "mem/kheap.h"
#include "fs/file.h"

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

    fclose(fd);
out:
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

    struct task* task = 0;
    struct process* _process;
    void* program_stack_ptr = 0;

    if(process_get(process_slot) != 0)
    {
        r = -EISTKN;
        goto out;
    }

    _process = kzalloc(sizeof(struct process));

    if(!_process)
    {
        r = -ENOMEM;
        goto out;
    }

    process_init(_process);

    r = process_load_data(filename, _process);

out: 
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

