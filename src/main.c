#include "vga.h"
#include "stdio.h"

#if defined(__linux__)
#error "Need to use cross compiler"
#endif

#if !defined(__i386__)
#error "Must be x86 cross compiler"
#endif

int kernel_main(void) {
    terminal_initialize();
    for (int i = 0; ; i++) {
        printf("num %d\n", i);
    }
    return 0;
}
