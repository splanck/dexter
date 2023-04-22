#include "../lib/string.h"

int strlen(const char* s) {
    int len = 0;

    while(s[len])
        len++;

    return len;
}

int strnlen(const char* s, int max) {
    int x = 0;

    for(x = 0; x < max; x++)
        if(s[x] == 0) 
            break;

    return x;
}

int tonumericdigit(char c) {
    return c - 48;
}

bool isdigit(char c) {
    return c >= 48 && c <= 57;
}
