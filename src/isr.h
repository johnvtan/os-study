#ifndef __ISR_H__
#define __ISR_H__

#include <stdint.h>

#define IRQ_OFFSET 32

struct registers {
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
};

typedef void (*isr_t)(struct registers);

void interrupts_init(void);
void isr_handler(struct registers regs);
void irq_handler(struct registers regs);
void register_interrupt_handler(uint8_t index, isr_t handler);
#endif
