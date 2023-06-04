#include "dev/keyboard.h"
#include "sys/status.h"
#include "sys/kernel.h"
#include "proc/process.h"
#include "proc/task.h"

static struct keyboard* keyboard_list_head = 0;
static struct keyboard* keyboard_list_tail = 0;

static int keyboard_get_tail_index(struct process* process)
{
    return process->keyboard.tail % sizeof(process->keyboard.buffer);
}

void keyboard_backspace(struct process* process)
{
    process->keyboard.tail--;
    int real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = 0x00;
}

void keyboard_push(char c)
{
    struct process* process = process_current();

    if(!process)
    {
        return;
    }

    int real_index = keyboard_get_tail_index(process);

    process->keyboard.buffer[real_index] = c;
    process->keyboard.tail++;
}

char keyboard_pop()
{
    if(!task_current())
    {
        return 0;
    }

    struct process* process = task_current()->process;

    int real_index = process->keyboard.head % sizeof(process->keyboard.buffer);
    char c = process->keyboard.buffer[real_index];

    if(c == 0x00)
    {
        return 0;
    }

    process->keyboard.buffer[real_index] = 0;
    process->keyboard.head++;

    return c;
}

int keyboard_insert(struct keyboard* keyboard)
{
    int r = 0;

    if(keyboard->init == 0)
    {
        r = -EINVARG;
    }

    if(keyboard_list_tail)
    {
        keyboard_list_tail->next = keyboard;
        keyboard_list_tail = keyboard;
    }
    else
    {
        keyboard_list_head = keyboard;
        keyboard_list_tail = keyboard;
    }

    r = keyboard->init();
out:
    return r;
}

void keyboard_init()
{

}


