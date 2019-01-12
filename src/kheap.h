#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>

void *kmalloc_a(uint32_t size);
void *kmalloc_p(uint32_t size, uint32_t *phys);
void *kmalloc_ap(uint32_t size, uint32_t *phys);
void *kmalloc(uint32_t size);

extern uint32_t placement_address;

#endif
