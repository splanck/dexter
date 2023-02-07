#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stddef.h>

#define DBG_MODE 1
#define VGA_WIDTH 80
#define VGA_HEIGHT 20

uint16_t terminal_make_char(char c, char colour);
void terminal_putchar(int x, int y, char c, char colour);
void terminal_writechar(char c, char colour);
void terminal_initialize();
void print(const char* str);

#endif
