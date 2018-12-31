#include "stdio.h"
#include "isr.h"

void isr_handler(struct registers regs) {
    printf("received interrupt: %d\n", regs.int_no);
}
