#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "console.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init() {
    int total_table_entries = DEXTER_HEAP_SIZE_BYTES / DEXTER_HEAP_BLOCK_SIZE;

    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(DEXTER_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = (void*)(DEXTER_HEAP_ADDRESS + DEXTER_HEAP_SIZE_BYTES);

    int res = heap_create(&kernel_heap, (void*)(DEXTER_HEAP_ADDRESS), end, 
        &kernel_heap_table);

    if(res < 0)
        cprint("Heap creation failed.\n", 12);
}