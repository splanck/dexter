#include "../lib/utility.h"

static unsigned int saved_seed = 0;

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
