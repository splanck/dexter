#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init();  

#endif