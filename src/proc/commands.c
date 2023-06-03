#include "proc/commands.h"
#include "proc/task.h"
#include "sys/idt.h"
#include "lib/console.h"

void* isr80h_command0_sum(struct interrupt_frame* frame)
{
    int v2 = (int) task_get_stack_item(task_current(), 1);
    int v1 = (int) task_get_stack_item(task_current(), 0);

    return (void*)(v1 + v2);
}

void* isr80h_command1_print(struct interrupt_frame* frame)
{
    void* user_space_msg_buf = task_get_stack_item(task_current(), 0);
    char buf[1024];

    copy_string_from_task(task_current(), user_space_msg_buf, buf, sizeof(buf));

    print(buf);

    return 0;
}

