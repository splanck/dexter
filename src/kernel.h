#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

void* kmalloc(size_t size);
void kfree(void* ptr);
void kernel_main();

#endif