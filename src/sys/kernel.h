#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

void kernel_main();
void kernel_panic();

#define ERROR(value) (void*)value
#define ERROR_I(value) (int)value
#define ISERR(value) ((int)value) < 0

#endif