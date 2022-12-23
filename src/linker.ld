ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 1M
    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.nodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}