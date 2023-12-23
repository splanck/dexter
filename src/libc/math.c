#include "libc/math.h"

static unsigned int saved_seed = 0;

int abs(int n) 
{
    return (n < 0) ? -n : n;
}

double ceil(double x) 
{
    int xi = (int)x; // Cast to integer to truncate
    return (x > 0 && x != xi) ? xi + 1 : xi;
}

double floor(double x) 
{
    int xi = (int)x; // Cast to integer to truncate
    return (x < 0 && x != xi) ? xi - 1 : xi;
}

double pow(double base, int exp) 
{
    double result = 1.0;

    for(int i = 0; i < abs(exp); i++) 
    {
        result *= base;
    }

    return exp < 0 ? 1 / result : result;
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

double round(double x)
 {
    if (x < 0.0) 
    {
        return -round(-x);
    }
    
    double floorX = floor(x);
    
    if (x - floorX < 0.5) 
    {
        return floorX;
    } 
    else if (x - floorX > 0.5) 
    {
        return floorX + 1;
    } 
    else 
    {
        // If the number is exactly halfway between two integers
        return ((int)floorX % 2 == 0) ? floorX : floorX + 1;
    }
}

double sqrt(double num) 
{
    double guess = num / 2.0;
    double epsilon = 0.000001; // Precision of the result
    
    while (abs(guess * guess - num) > epsilon) 
    {
        guess = (guess + num / guess) / 2.0;
    }
    
    return guess;
}

void srand(unsigned int seed) 
{
    saved_seed = seed;
}
