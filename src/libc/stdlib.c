#include "libc/stdlib.h"
#include "mem/heap.h"

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
