#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H

#include <stdint.h>
#include <stddef.h>

#define PS2_PORT 0x64
#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE

int classic_keyboard_init();
struct keyboard* clasic_init();
uint8_t classic_keyboard_scancode_to_char(uint8_t scancode);

#endif