#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00
#define HEAP_BLOCK_HAS_NEXT 0b100000000
#define HEAP_BLOCK_IS_FIRST 0b010000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heap_table 
{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total;
};

struct heap 
{
    struct heap_table* table;
    void* saddr;
};

static int heap_validate_table(void* ptr, void* end, struct heap_table* table);
static bool heap_validate_alignment(void* ptr);
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);
void* heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap* heap, void* ptr);

#endif
