#include "../mem/heap.h"
#include "../mem/kheap.h"
#include "../mem/memory.h"
#include "../lib/console.h"
#include "../sys/config.h"

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

void* kmalloc(size_t size) {
    return heap_malloc(&kernel_heap, size);
}

void kfree(void* ptr) {
    heap_free(&kernel_heap, ptr);
}

void* kzalloc(size_t size) {
    void* ptr = kmalloc(size);

    if(!ptr) return 0;
    
    memset(ptr, 0x00, size);

    return ptr;
}
