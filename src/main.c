#include "vga.h"

#if defined(__linux__)
#error "Please work"
#endif

#if !defined(__i386__)
#error "did i do this right"
#endif

int kernel_main(void) {
    terminal_initialize();
    terminal_puts("Hello, kernel world!\n");
    return 0;
}
