#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

inline void outb(uint16_t port, uint8_t data);
inline uint8_t inb(uint16_t port);
inline void io_wait(void);
#endif
