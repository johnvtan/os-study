MBOOT_PAGE_ALIGN equ 1<<0
MBOOT_MEM_INFO equ 1<<1
MBOOT_HEADER_MAGIC equ 0x1BADB002
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32] ; all instructions are 32 bit
[GLOBAL mboot] ; mboot is accessible from C
[EXTERN code] ; start of the .text section - loaded from linker script?
[EXTERN bss] ; start of the .bss section - loaded from linker script?
[EXTERN end] ; end of the last loadable section

mboot:
; this is the multiboot header for grub, which specifies the environment for the kernel when it boots and allows the
; kernel to query the environment its in
; this header must be in the first 4Kb of the kernel
; dd is an NASM command that lets us put constants at specific memory locations
    dd MBOOT_HEADER_MAGIC ; GRUB will search for this value
    dd MBOOT_HEADER_FLAGS ; tells grub how to load the file
    dd MBOOT_CHECKSUM

    dd mboot ; location of this descriptor
    dd code ; start of code
    dd bss ; start of .data section
    dd end ; end of kernel
    dd start ; kernel entry point (initial EIP)

[GLOBAL start]
[EXTERN main] ; This is the entry point of our C code

start:
    push ebx ; Load multiboot header location

    ; execute the kernel:
    cli ; disable interrupts
    call main ; call the main() function
    jmp $ ; enter an infinite loop (stops the processor from continuing execution)
