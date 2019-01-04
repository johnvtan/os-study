#include "vga.h"
#include "gdt.h"
#include "stdio.h"
#include "idt.h"
#include "timer.h"
#include "isr.h"

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
    interrupts_init();
    asm volatile("sti");
    timer_init(100);
    printf("All init\n");
    /*
    asm volatile("int $0x21");
    asm volatile("int $0x20");
    asm volatile("int $0x2F");
    */
    for (;;) {
        asm("hlt");
    }
    return 0;
}
