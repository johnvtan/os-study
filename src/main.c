#include "vga.h"
#include "gdt.h"
#include "stdio.h"
#include "idt.h"

#if defined(__linux__)
#error "Need to use cross compiler"
#endif

#if !defined(__i386__)
#error "Must be x86 cross compiler"
#endif

int kernel_main(void) {
    gdt_init();
    idt_init();
    terminal_initialize();
    printf("OK\n");
	asm volatile("int $0x3");
    asm volatile("int $0x4");
    return 0;
}
