.global gdt_flush

gdt_flush:
    mov 4(%esp), %eax # pointer to gdt is passed as first parameter
    lgdt (%eax)
    
    mov $0x10, %ax # 0x10 is the offset to the data segment

    # load 0x10 into all the 
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    jmp $0x08, $flush
flush:
    ret
