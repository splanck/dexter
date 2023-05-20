#include "mem/paging.h"
#include "mem/heap.h"
#include "mem/kheap.h"
#include "sys/status.h"

static uint32_t* current_directory = 0;

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk) 
{
    return chunk->directory_entry;
}

void paging_switch(uint32_t* directory) 
{
    paging_load_directory(directory);
    current_directory = directory;
}

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags) 
{
    uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    int offset = 0;

    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) 
    {
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE); 
        
        for(int x = 0; x < PAGING_TOTAL_ENTRIES_PER_TABLE; x++) 
        {
            entry[x] = (offset + (x + PAGING_PAGE_SIZE)) | flags;
        }

        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITEABLE;
    }

    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;

    return chunk_4gb;
}

bool paging_is_aligned(void* addr) 
{
    return ((uint32_t)addr % PAGING_PAGE_SIZE) == 0;
}

int paging_set(uint32_t* directory, void* virt, uint32_t val) 
{
    int r = 0;
    
    if(!paging_is_aligned(virt)) 
    {
        r = -EINVARG;
        goto out;
    }

    uint32_t directory_ix, table_ix = 0;
    r = paging_get_indexes(virt, &directory_ix, &table_ix);
    
    if(r < 0) 
    {
        goto out;
    }

    uint32_t entry = directory[directory_ix];
    uint32_t* table = (uint32_t*)(entry & 0xfffff000);

    table[table_ix] = val;
out:
    return r;
}

int paging_get_indexes(void* virtual_address, uint32_t* dir_ix_out, uint32_t* tbl_ix_out) 
{
    int r = 0;

    if(!paging_is_aligned(virtual_address)) 
    {
        r = -EINVARG;
        goto out;
    }

    *dir_ix_out = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *tbl_ix_out = ((uint32_t)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)
        / PAGING_PAGE_SIZE);
out:
    return r;
}