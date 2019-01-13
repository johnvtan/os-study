#include <stddef.h>
#include "strings.h"
#include "stdio.h"
#include "isr.h"
#include "io.h"
#include "pic.h"

static isr_t interrupt_handlers[256];

// something wrong with how interrupts are set up!!!
// if I don't memset the interrupt handlers, when the timer IRQ is called,
// it will call the isr handler with int_no = 6
// not being remapped??
void interrupts_init(void) {
    memset(interrupt_handlers, 0, sizeof(isr_t) * 256);
    // enable interrupts
    asm volatile("sti");
}

void isr_handler(struct registers regs) {
    if (interrupt_handlers[regs.int_no] != NULL) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    } else {
        printf("Unhandled isr: %d\n", regs.int_no);
    }
}

void irq_handler(struct registers regs) {
    pic_send_eoi(regs.int_no - 32);
    if (interrupt_handlers[regs.int_no] != NULL) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    } else {
        printf("Unhandled irq: %d\n", regs.int_no - 32);
    }
}

void register_interrupt_handler(uint8_t index, isr_t handler) {
    printf("registering irq %d\n", index);
    interrupt_handlers[index] = handler;
}
