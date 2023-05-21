#include "proc/task.h"
#include "mem/memory.h"
#include "mem/kheap.h"
#include "sys/status.h"
#include "sys/kernel.h"
#include "sys/config.h"

struct task* current_task = 0;
struct task* task_head = 0;
struct task* task_tail = 0;

struct task* task_current()
{
    return current_task;
}

struct task* task_new()
{
    int r = 0;

    struct task* task = kzalloc(sizeof(struct task));

    if(!task)
    {
        r = -ENOMEM;
        goto out;
    }

    r = task_init(task);

    if(r != DEXTER_ALL_OK)
    {
        goto out;
    }

    if(task_head == 0)
    {
        task_head = task;
        task_tail = task;

        goto out;
    }

    task_tail->next = task;
    task->previous = task;
    task_tail = task;

out:
    if(ISERR(r))
    {
        task_free(task);
        return ERROR(r);
    }

    return task;
}

static void task_list_remove(struct task* task)
{
    if(task->previous)
    {
        task->previous->next = task->next;
    }

    if(task == task_head)
    {
        task_head = task->next;
    }

    if(task == task_tail)
    {
        task_tail = task->previous;
    }

    if(task == current_task)
    {
        current_task = task_get_next();
    }
}

struct task* task_get_next()
{
    if(!current_task->next)
    {
        return task_head;
    }
    else
    {
        return current_task->next;
    }
}

int task_free(struct task* task)
{
    paging_free_4gb(task->page_directory);
    task_list_remove(task);

    kfree(task);

    return 0;
}

int task_init(struct task* task)
{
    int r = 0;

    memset(task, 0, sizeof(struct task));
    task->page_directory = paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    if(!task->page_directory)
    {
        r = -EIO;
        goto out;
    }

    task->registers.ip = DEXTER_PROGRAM_VIRTUAL_ADDRESS;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.esp = DEXTER_PROGRAM_VIRTUAL_STACK_ADDRESS_START;
    
out:
    return r;
}
