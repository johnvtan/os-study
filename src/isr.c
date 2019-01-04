#include <stddef.h>
#include "stdio.h"
#include "isr.h"
#include "io.h"
#include "pic.h"

static isr_t interrupt_handlers[256] = {0};

void isr_handler(struct registers regs) {
    printf("received interrupt: %d\n", regs.int_no);
}

void irq_handler(struct registers regs) {
    pic_send_eoi(regs.int_no - 32);
    if (interrupt_handlers[regs.int_no] != NULL) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void register_interrupt_handler(uint8_t index, isr_t handler) {
    interrupt_handlers[index] = handler;
}
