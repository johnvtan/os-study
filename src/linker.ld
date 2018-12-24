ENTRY(start)
SECTIONS
{
    .text 0x100000 :
    {
        code = .;
        *(.text)
        . = ALIGN(0x1000);
    }

    .data :
    {
        data = .;
        *(.data)
        *(.rodata)
        . = ALIGN(0x1000);
    }

    .bss :
    {
        bss = .;
        *(.bss)
        . = ALIGN(0x1000);
    }

    end = .;
}
