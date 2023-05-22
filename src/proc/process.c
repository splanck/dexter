#include "proc/process.h"
#include "proc/task.h"
#include "sys/config.h"
#include "sys/status.h"
#include "mem/memory.h"

struct process* current_process = 0;
static struct process* processes[DEXTER_MAX_PROCESSES] = {};

struct process* process_current()
{
    return current_process;
}

static void process_init(struct process* process)
{
    memset(process, 0, sizeof(struct process));
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

out: 
    return r;
}



