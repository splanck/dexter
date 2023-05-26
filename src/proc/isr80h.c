#include "proc/isr80h.h"
#include "proc/commands.h"
#include "sys/idt.h"

void isr80h_register_commands()
{
    isr80h_register_command(SYSTEM_COMMAND0_SUM, isr80h_command0_sum);
}

