#include "vga.h"
#include "gdt.h"
#include "stdio.h"
#include "idt.h"
#include "timer.h"
#include "isr.h"
#include "paging.h"
#include "panic.h"

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
    printf("cool\n");
    //interrupts_init();
    paging_init();
    //timer_init(100);
    printf("All init\n");
    uint32_t *ptr = (uint32_t*)0xA0000000; 
    uint32_t do_page_fault = *ptr;
    PANIC("end of kernel\n");
    return 0;
}
