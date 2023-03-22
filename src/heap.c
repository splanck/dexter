#include "heap.h"
#include "memory.h"
#include "status.h"
#include "config.h"

static bool heap_validate_alignment(void* ptr) {
    return (unsigned int)ptr % DEXTER_HEAP_BLOCK_SIZE == 0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table) {
    int res = 0;

    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end)) {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
};
