#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>
#include "isr.h"

struct page {
    uint32_t present :1;
    uint32_t rw :1;
    uint32_t user :1;
    uint32_t accessed :1;
    uint32_t dirty :1;
    uint32_t unused :7;
    uint32_t frame :20;
};

struct page_table {
    struct page pages[1024];
};

struct page_directory {
    struct page_table *tables[1024];
    uint32_t tables_physical[1024];
    uint32_t physical_addr;
};

void paging_init(void);
void switch_page_directory(struct page_directory *new_directory);
struct page *get_page(uint32_t address, int make_new, struct page_directory *dir);
void page_fault(struct registers regs);

#endif
