#include "timer.h"
#include "isr.h"
#include "stdio.h"
#include "io.h"

#define TIMER_BASE_FREQ (1193180U)
#define TIMER_CMD_PORT (0x43)
#define TIMER_DATA_PORT (0x40)
#define TIMER_REPEAT_MODE (0x36)

static uint32_t tick;

static void timer_callback(struct registers regs) {
    (void)regs;
    tick++;
    printf("Tick: %u\n", tick);
}

void timer_init(uint32_t frequency) {
    tick = 0;

    register_interrupt_handler(32, &timer_callback);

    outb(TIMER_CMD_PORT, TIMER_REPEAT_MODE);

    uint32_t divisor = TIMER_BASE_FREQ / frequency;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(TIMER_DATA_PORT, low);
    outb(TIMER_DATA_PORT, high);
}
