#include "kheap.h"
#include "stdio.h"

extern uint32_t __end;
uint32_t placement_address = (uint32_t)&__end;

static void *kmalloc_internal(uint32_t size, int align, uint32_t *phys) {
    printf("internal\n");
    if (align == 1 && (placement_address & 0xFFFF0000)) {
        placement_address &= 0xFFFF0000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint32_t rv = placement_address;
    placement_address += size;
    printf("I should be good\n");
    return (void*)rv;
}

// aligned kmalloc
void *kmalloc_a(uint32_t size) {
    return kmalloc_internal(size, 1, 0);
}

void *kmalloc_p(uint32_t size, uint32_t *phys) {
    return kmalloc_internal(size, 0, phys);
}

void *kmalloc_ap(uint32_t size, uint32_t *phys) {
    return kmalloc_internal(size, 1, phys);
}

void *kmalloc(uint32_t size) {
    void *rv = kmalloc_internal(size, 0, 0);
    printf("done\n");
    return rv;
}
