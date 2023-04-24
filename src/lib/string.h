#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

int strlen(const char* s);
int strnlen(const char* s, int max);
int tonumericdigit(char c);
bool isdigit(char c);
char* strcpy(char *destination, char *source);

#endif