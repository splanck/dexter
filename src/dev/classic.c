#include "dev/classic.h"
#include "dev/keyboard.h"
#include "sys/io.h"

static uint8_t keyboard_scan_set_one[] =
{
    0x00, 0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08,
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0x0d,
    0x00, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '~', 0x00,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0x00,
    '*', 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '.'
};

struct keyboard classic_keyboard = 
{
    .name = {"Classic"},
    .init = classic_keyboard_init
};

uint8_t classic_keyboard_scancode_to_char(uint8_t scancode)
{
    size_t size_of_keyboard_set_one = sizeof(keyboard_scan_set_one) / sizeof(uint8_t);

    if(scancode > size_of_keyboard_set_one)
    {
        return 0;
    }

    char c = keyboard_scan_set_one[scancode];

    return c;
}

int classic_keyboard_init()
{
    outb(PS2_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
    
    return 0;
}

struct keyboard* clasic_init()
{
    return &classic_keyboard;
}

void classic_keyboard_interrupt_handler()
{
    
}