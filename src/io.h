#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t rv;
    asm volatile("inb %1, %0" : "=a"(rv) : "Nd"(port));
    return rv;
}

static inline void io_wait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}
#endif
