#ifndef __VGA_H__
#define __VGA_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "strings.h"

void terminal_initialize(void);
void terminal_putc(char c);
void terminal_puts(const char *data);
#endif
