#include "vga.h"

#define VGA_WIDTH (80)
#define VGA_HEIGHT (25)
#define VGA_BUFFER_ADDRESS (0xB8000)

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GRAY = 7,
    VGA_COLOR_DARK_GRAY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) VGA_BUFFER_ADDRESS;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

static void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_scroll() {
    if (terminal_row > VGA_HEIGHT) {
        // copy all rows to the row above it
        for (int i = 0; i < VGA_HEIGHT*VGA_WIDTH; i++) {
            terminal_buffer[i] = terminal_buffer[i + 80];
        }
        // fill last line with spaces
        for (int i = 0; i < 80; i++) {
            terminal_putentryat(' ', terminal_color, VGA_HEIGHT, i);             
        }
        terminal_row = VGA_HEIGHT;
    }
}

void terminal_putc(char c) {
    if (c == '\n') {
        terminal_column = 0;
        ++terminal_row;
    } else if (c == '\r') {
        terminal_column = 0;
    } else if (c == 0x08 && terminal_column > 0) {
        // backspace
        --terminal_column;
    }
    else if (c == 0x9) {
        // tab character
        terminal_column = (terminal_column + 8) & ~(8-1);
    }
    else if (c >= ' ' ) {
        // any other printable character
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
    terminal_scroll();
}

static void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putc(data[i]);
    }
}

void terminal_puts(const char* data) {
    terminal_write(data, strlen(data));
}
