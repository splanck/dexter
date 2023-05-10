#include "../lib/string.h"

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

bool isdigit(char c) 
{
    return c >= 48 && c <= 57;
}

int istrncmp(const char *s1, const char *s2, size_t n) 
{
    int f, l;

    do 
    {
        if ((f = toupper((unsigned char)*s1)) == (l = toupper((unsigned char)*s2))) 
        {
            ++s1;
            ++s2;
        } 
        else
        {
            break;
        }
    } while (--n > 0);
    
    return (f - l);
}

char* strcpy(char *destination, char *source) 
{
    while (*source != '\0') 
    {
        *destination = *source;
        source++;
        destination++;
    }

    *destination = '\0';
    
    return destination;
}

int strcmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        ++s1;
        ++s2;
    }
    
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
 {
    while (n && *s1 && (*s1 == *s2)) 
    {
        ++s1;
        ++s2;
        --n;
    }
    
    if (n == 0) 
    {
        return 0;
    } 
    else 
    {
        return (* (unsigned char *)s1 - * (unsigned char *)s2);
    }
}

char* strncpy(char *dest, const char *src, size_t n) 
{
    size_t i;
    
    for (i = 0; i < n && src[i] != '\0'; i++) 
    {
        dest[i] = src[i];
    }
    
    for ( ; i < n; i++) 
    {
        dest[i] = '\0';
    }
    
    return dest;
}


int strlen(const char* s) 
{
    int len = 0;

    while(s[len])
    {
        len++;
    }

    return len;
}

int strnlen(const char* s, int max) 
{
    int x = 0;

    for(x = 0; x < max; x++)
    {
        if(s[x] == 0)
        {
            break;
        } 
    }

    return x;
}

size_t strnlen_terminator(const char *s, size_t maxlen, char terminator) 
{
    size_t len;
    for (len = 0; len < maxlen && s[len] != terminator; ++len);

    return len;
}

int tonumericdigit(char c) 
{
    return c - 48;
}

char tolower(char c) 
{
    if (c >= 'A' && c <= 'Z') 
    {
        return c + ('a' - 'A');
    }
    
    return c;
}

char toupper(char c) 
{
    if (c >= 'a' && c <= 'z') 
    {
        return c - 'a' + 'A';
    } 
    else 
    {
        return c;
    }
}
