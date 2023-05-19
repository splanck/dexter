#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

int atoi(const char *str);
bool isdigit(char c);
int istrncmp(const char *s1, const char *s2, size_t n);
void strcat(char *dest, char *src);
char* strcpy(char *destination, char *source);
int strcmp(const char *s1, const char *s2);
int strlen(const char* s);
int strncmp(const char *s1, const char *s2, size_t n);
char* strncpy(char *dest, const char *src, size_t n);
int strnlen(const char* s, int max);
size_t strnlen_terminator(const char *s, size_t maxlen, char terminator);
int tonumericdigit(char c);
char tolower(char c);
char toupper(char c);

#endif