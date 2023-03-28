#include "paging.h"
#include "heap.h"
#include "kheap.h"

static uint32_t* current_directory = 0;

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk) {
    return chunk->directory_entry;
}

void paging_switch(uint32_t* directory) {
    paging_load_directory(directory);
    current_directory = directory;
}

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags) {
    uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    int offset = 0;

    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE); 
        
        for(int x = 0; x < PAGING_TOTAL_ENTRIES_PER_TABLE; x++) {
            entry[x] = (offset + (x + PAGING_PAGE_SIZE)) | flags;
        }

        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITEABLE;
    }

    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;

    return chunk_4gb;
}