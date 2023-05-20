#ifndef KERNEL_H
#define KERNEL_H

#define DEBUG
#define VERBOSE

#include <stdint.h>
#include <stddef.h>

void kernel_main();
void panic(const char* msg);

#define ERROR(value) (void*)value
#define ERROR_I(value) (int)value
#define ISERR(value) ((int)value) < 0

#endif