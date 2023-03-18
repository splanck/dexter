#include "console.h"
#include "kernel.h"
#include "string.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour) {
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour) {
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour) {
    if (c == '\n') {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    else {
        terminal_putchar(terminal_col, terminal_row, c, colour);
        terminal_col += 1;
    
        if (terminal_col >= VGA_WIDTH)
        {
            terminal_col = 0;
            terminal_row += 1;
        }   
    }
}

void terminal_initialize() {
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;

    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}

void cprint(const char* str, int c) {
    size_t len = strlen(str);

    for (int i = 0; i < len; i++) {
        terminal_writechar(str[i], c);
    }
}

void print(const char* str) {
    cprint(str, 15);
}
