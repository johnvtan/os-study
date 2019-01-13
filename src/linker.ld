ENTRY(_start)
SECTIONS
{
    .text 0x100000 :
    {
        *(.multiboot)
        *(.text)
        . = ALIGN(4096);
    }

    .rodata :
    {
        *(.rodata)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        . = ALIGN(4096);
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
        . = ALIGN(4096);
    }

    __end = .;
}
