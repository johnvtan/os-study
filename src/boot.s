.set ALIGN, 1<< 0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# defines multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# defines location and size of the stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KB allocated for stack
stack_top:

# entry point to the kernel
.section .text
.global _start
.type _start, @function
_start:
    # at this point, we're in 32 bit protected mode and ints and paging are disabled
    # set up the stack by moving the stack_top location into esp
    mov $stack_top, %esp

    # initialize processor state here
    call kernel_main

    # if we return from kernel main, put computer into infinite loop
    cli
1:  hlt
    jmp 1b

.size _start, . - _start
