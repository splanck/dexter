#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt
{
    uint16_t segment;
    uint16_t base_first;
    uint8_t base;
    uint8_t access;
    uint8_t high_flags;
    uint8_t base_24_31_bits;
};



#endif