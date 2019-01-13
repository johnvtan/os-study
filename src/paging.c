#include "paging.h"
#include "panic.h"
#include "kheap.h"
#include "strings.h" 
#include "stdio.h"
static uint32_t *frames; static uint32_t num_frames; 
static struct page_directory *kernel_directory;
static struct page_directory *current_directory;

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

// set bit in frame bitmap
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (1 << off);
}

// clear bit in frame bitmap
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(1 << off);
}

static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (1 << off));
} 

static int first_frame(void) {
    uint32_t i;
    uint32_t j;
    for (i = 0; i < INDEX_FROM_BIT(num_frames); i++) {
        for (j = 0; j < 32; j++) {
            uint32_t test = frames[i] & (1 << j);
            if (test == 0) {
                return (i * 32) + j;
            }
        }
    }
    // no free frames
    return -1;
}

int alloc_frame(struct page *page, int is_kernel, int writeable) {
    if (page->frame != 0) {
        // page is already allocated, so do nothing
        return -1;
    }
    int free_idx = first_frame();
    if (free_idx == -1) {
        PANIC("No free frames");
    }
    // otherwise, we got a free frame
    set_frame(free_idx * 0x1000); // mark it as used in the bitmap
    page->present = 1;
    page->rw = writeable ? 1 : 0; 
    page->user = is_kernel ? 0 : 1;
    page->frame = free_idx;
    return 0;
}

int free_frame(struct page *page) {
    uint32_t frame = page->frame;
    if (frame == 0) {
        // frame wasn't allocated in the first place
        return -1;
    }
    clear_frame(frame); // clear frame in bitmap
    page->frame = 0;    // set this page to not point to any frame
    return 0;
}

void paging_init(void) {
    uint32_t mem_end_page = 0x10000000;
    num_frames = mem_end_page / 0x1000;

    // allocate memory for page bitmap
    uint32_t bitmap_size = INDEX_FROM_BIT(num_frames);
    frames = (uint32_t *)kmalloc(bitmap_size);
    memset(frames, 0, bitmap_size);
    
    // allocate memory from kernel page directory
    kernel_directory = (struct page_directory *)kmalloc_a(sizeof(struct page_directory));
    memset(kernel_directory, 0, sizeof(struct page_directory));
    current_directory = kernel_directory;

    uint32_t i = 0;
    while (i < placement_address) {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    register_interrupt_handler(14, page_fault);
    switch_page_directory(kernel_directory);
} 

void switch_page_directory(struct page_directory *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir->tables_physical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

struct page *get_page(uint32_t address, int make_new, struct page_directory *dir) {
    address /= 0x1000;
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) {
        // page table already exists, return that page
        return (&dir->tables[table_idx]->pages[address % 1024]);
    } else if (make_new) {
        // allocate memory for new page table and zero it out
        uint32_t temp;
        dir->tables[table_idx] = (struct page_table*)kmalloc_ap(sizeof(struct page_table), &temp);
        dir->tables_physical[table_idx] = temp | 0x7; // PRESENT, RW, US
        return (&dir->tables[table_idx]->pages[address % 1024]);
    } else {
        return 0;
    }
}

void page_fault(struct registers regs) {
    uint32_t fault_address;
    asm volatile("mov %%cr2, %0" : "=r"(fault_address));
    int present = !(regs.err_code & 0x1);
    int rw = regs.err_code & 0x2;
    int user = regs.err_code & 0x4;
    int reserved = regs.err_code & 0x8;
    int id = regs.err_code & 0x10;

    printf("----------------Page fault! : ");
    if (present) {
        printf("present; ");
    }
    if (rw) {
        printf("read only; ");
    }
    if (user) {
        printf("user mode; ");
    }
    if (reserved) {
        printf("reserved;------------\n");
    }
    printf("Tried to access address: 0x%x\n", fault_address);
    PANIC("**************PAGE FAULT***************");
}
