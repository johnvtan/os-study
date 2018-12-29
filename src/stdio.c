#include "stdio.h"
#include "strings.h"
#include "vga.h" 

// for now, we can't handle any more than 16 digit numbers
#define VSNPRINTF_MAX_DIGITS 16
#define PRINTF_MAX_BUF_SIZE 1024

void putc(const unsigned char c) {
    terminal_putc(c);
}

void puts(const unsigned char *str) {
    while ('\0' != *str) {
        putc(*str++);
    }
}

int printf(const char *fmt, ...) {
    int rv;
    va_list args;
    va_start(args, fmt);

    char buf[PRINTF_MAX_BUF_SIZE];
    rv = vsnprintf(buf, PRINTF_MAX_BUF_SIZE, fmt, args);
    puts((unsigned char*)buf);
    
    va_end(args);
    return rv;
}

int snprintf(char *str, size_t size, const char *fmt, ...) {
    if (size > PRINTF_MAX_BUF_SIZE) {
        return -1;
    }
    int rv;
    va_list args;
    va_start(args, fmt);
    rv = vsnprintf(str, size, fmt, args);
    va_end(args);
    return rv;
}

// for now, only handle %u, %c, %s
// returns size of str?
int vsnprintf(char *str, size_t size, const char *fmt, va_list args) {
    if (NULL == str) {
        return -1;
    }

    char curr = 0;
    int fmt_next = 0;
    size_t arg_len = 0;
    char replace[VSNPRINTF_MAX_DIGITS];
    char *str_start = str; 

    while ('\0' != (curr = *fmt++) && size--) {
        if (fmt_next) {
            // curr should be u or x or some format character
            switch (curr) {
                case 'd':   // for now, %d and %u mean the same thing.
                            // it's annoying when I test and I forget I don't have %d, only %u
                case 'u':
                    // TODO: need to subtract the number of bytes that are copied here
                    // from the total
                    numtostr(va_arg(args, unsigned int), 10, replace, &arg_len);
                    memcpy(str, replace, arg_len);
                    str += arg_len;
                    break;
                case 'x':
                    numtostr(va_arg(args, unsigned int), 16, replace, &arg_len);
                    memcpy(str, replace, arg_len);
                    str += arg_len;
                    break;
                case 'c':
                    // va_arg() needs the type passed in to be int and then cast to (char)
                    // can't, e.g., call va_arg(args, char) - otherwise it doesn't return!
                    *str++ = (char)va_arg(args, int);
                    break;
                case 's': ; // empty statement since declarations can't follow labels in C
                    char *new_str = va_arg(args, char *);
                    strcpy(str, new_str);
                    str += strlen(new_str);
                    break;
                default:
                    break;
            }
            fmt_next = 0;
        } else if ('%' == curr) {
            fmt_next = 1;
        } else {
            *str++ = curr;
        }
    }
    // need to add null terminating character
    *str = '\0';

    // return size of formatted string
    return (str - str_start);
}
