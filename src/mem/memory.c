#include "../mem/memory.h"

int memset(void* ptr, int c, size_t size) 
{
    char* c_ptr = (char*)ptr;

    for(int i = 0; i < size; i++)
    {
        c_ptr[i] = (char)c;
    }

    return ptr;
}

int memcmp(const void *s1, const void *s2, size_t n) 
{
    const unsigned char *p1 = s1, *p2 = s2;

    while(n--) 
    {
        if(*p1 != *p2) 
        {
            return (*p1 > *p2) ? 1 : -1;
        }

        p1++;
        p2++;
    }

    return 0;
}

int memcpy(void *dest, const void *src, size_t n) 
{
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    for (int i = 0; i < n; i++)
    {
        cdest[i] = csrc[i];
    }

    return 0;
}
