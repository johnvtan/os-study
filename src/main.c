#include "vga.h"
#include "stdio.h"

#if defined(__linux__)
#error "Please work"
#endif

#if !defined(__i386__)
#error "did i do this right"
#endif

int kernel_main(void) {
    terminal_initialize();
    printf("Hello, kernel world!\nThis is your\nmaster %s %d", "this is a test string", 22);
    return 0;
}
