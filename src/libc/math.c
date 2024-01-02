#include "libc/math.h"



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


