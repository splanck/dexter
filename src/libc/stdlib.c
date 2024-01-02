#include "mem/heap.h"
#include "libc/stdlib.h"
#include "sys/config.h"

static unsigned int saved_seed = 0;

int atoi(const char *str) 
{
    int r = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-') 
    {
        sign = -1;
        i++;
    }

    for (; str[i] != '\0'; ++i) 
    {
        r = r * 10 + str[i] - '0';
    }

    return sign * r;
}

void* calloc(struct heap* heap, size_t num, size_t size) 
{
    // Calculate the total size needed
    size_t totalSize = num * size;
    
    // Avoid size overflow
    if (size != 0 && totalSize / size != num) 
    {
        // Handle the error appropriately: you might want to return NULL or handle it differently
        return NULL;
    }

    // Allocate memory using your heap_malloc or equivalent function
    void* block = heap_malloc(heap, totalSize);
    if (block != NULL) 
    {
        // Initialize allocated memory to zero
        memset(block, 0, totalSize);
    }
    
    return block;
}

void free(struct heap* heap, void* ptr) 
{
    if (ptr == NULL) 
    {
        // Standard behavior is to do nothing on NULL pointer
        return;
    }
    
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
    // Consider coalescing free blocks or other optimizations
}

bool isdigit(char c) 
{
    return c >= 48 && c <= 57;
}

void* malloc(struct heap* heap, size_t size) 
{
    if (size == 0) 
    {
        // Standard behavior is to return NULL for 0 size
        return NULL;
    }
    
    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / DEXTER_HEAP_BLOCK_SIZE;

    void* block = heap_malloc_blocks(heap, total_blocks);
    if (block == NULL) 
    {
        // Handle allocation failure
        return NULL;
    }
    // Consider initializing memory or leaving it uninitialized based on policy
    return block;
}

void* memmove(void* dest, const void* src, size_t n) 
{
    unsigned char* d = dest;
    const unsigned char* s = src;

    // If the source and destination overlap, it could result in
    // data corruption if we just copied the data normally. To
    // avoid this, we need to decide whether to copy forwards
    // or backwards.

    if (d < s) 
    {
        // If the destination is before the source, we can copy
        // forward safely.
        for (size_t i = 0; i < n; i++) 
        {
            d[i] = s[i];
        }
    } 
    else 
    {
        // If the destination is after the source, we need to copy
        // backwards to prevent overwriting the data we want to copy.
        for (size_t i = n; i != 0; i--) 
        {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

void* memset(void* ptr, int c, size_t size)
{
    char* c_ptr = (char*) ptr;
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = (char) c;
    }
    return ptr;
}

int memcmp(void* s1, void* s2, int count)
{
    char* c1 = s1;
    char* c2 = s2;

    while(count-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }

    return 0;
}

void* memcpy(void* dest, void* src, int len)
{
    char *d = dest;
    char *s = src;

    while(len--)
    {
        *d++ = *s++;
    }

    return dest;
}

int rand(void) 
{
    static unsigned int seed = 1;
    unsigned int s = 0;

    if(saved_seed == 0)
    {
        s = seed;
    }
    else
    {
        s = saved_seed;
    }

    s = s * 1103515245 + 12345;

    return (unsigned int)(s / 65536) % 32768;
}

void srand(unsigned int seed) 
{
    saved_seed = seed;
}
