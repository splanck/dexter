#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>
#include <stdbool.h>
#include "mem/heap.h"

int atoi(const char *str);
void* calloc(struct heap* heap, size_t num, size_t size);
void free(struct heap* heap, void* ptr);
bool isdigit(char c);
void* malloc(struct heap* heap, size_t size);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* ptr, int c, size_t size);
int memcmp(void* s1, void* s2, int count);
void* memcpy(void* dest, void* src, int len);
int rand(void);
void srand(unsigned int seed);

#endif