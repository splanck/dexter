#ifndef COMMANDS_H
#define COMMANDS_H

struct interrupt_frame;
void* isr80h_command0_sum(struct interrupt_frame* frame);

#endif