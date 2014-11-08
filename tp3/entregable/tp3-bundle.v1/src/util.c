/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "util.h"
#include "screen.h"
#include <stdarg.h>

unsigned int strlen(const char *s) {
    unsigned int len = 0;

    while(*s != 0) {
        len++;
        s++;
    }
    return len;
}

void putc(char c, unsigned int col, unsigned int row) {
    // avoid black print bug
    unsigned char attr = getFormat(C_FG_WHITE, 0, C_BG_BLACK, 0);

    if (c == '\n') putc('\r', col, row);

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    p[row][col].c = c;
    p[row][col].a = attr;

}

int digits(unsigned int n, int base) {
    int digits = 0;
    while(n >= base) {
        n = n / 16;
        digits++;
    }

    return digits + 1;
} 

void print_intb(unsigned int n, int base, unsigned int col, unsigned int row) {
    // check base <= 16
    char numbers[16] = "0123456789ABCDEF";

    int d = digits(n, base);
    do {
        putc(numbers[n % base], col+d-1, row);
        n /= base;
        d--;
    } while (n > 0);
}

void print_int(unsigned int n, unsigned int col, unsigned int row) {
    print_intb(n, 10, col, row);
}


void printf(unsigned int col, unsigned int row, const char *format, ...) {

    va_list args;
    va_start(args, format);

    int nextArg = 0; // find a better name for this.
    int n;
    char *s;
    while(*format != 0) {
        if (nextArg == 0) {
            if (*format == '%') {
                nextArg = 1;
                format++;
                continue;
            }
            putc(*format, col, row);
            col++;
        } else {
            switch(*format) {
                case 'd':
                    n = va_arg(args,int);
                    print_int(n, col, row);
                    col += digits(n, 10);
                    break;
                case 'h':
                    n = va_arg(args,int);
                    print_string("0x", col, row, 0);
                    print_hex(n, col+2, row, 0);
                    col += digits(n, 16) + 2;
                    break;
                case 'p':
                    n = va_arg(args, unsigned int);
                    print_string("0x",col, row,0);
                    print_hex(n, col+2, row, 0);
                    col += digits(n, 16) + 2;
                    break;
                case 's':
                    s = va_arg(args, char*);
                    if (s == 0) {
                        print_string("(null)", col, row, 0);
                        col += 6;
                    } else {
                        print_string(s, col, row, 0);
                        col += strlen(s);
                    }
                    break;
                default:
                    print_string("Unknown format!", col, row, 0);
            }
        }
        
        if (col == 80) {
            col = 0;
            row++;
        }
        nextArg = 0;
        format++;
    }
    va_end(args);
}


void print_backtrace(unsigned int col, unsigned int row) {
    int i, ebp;

    asm("movl %%ebp, %0;" :"=r"(ebp));
    for(i = 0; i < 10 && ebp != 0; i++) {
        printf(col, row, "Call %d: 0x%d", i, ebp);
        asm("movl (%%ebp), %0;" :"=r"(ebp));
        row++;
    }
}

void* memcpy(void* src, void* dst, int length) {

    char* psrc = (char*) src;
    char* pdst = (char*) dst;

    while(length--) {
        *pdst++ = *psrc++;
    }

    return dst;
}

void* memset(void* src, int c, int n) {

    char* psrc = (char*) src;

    while(n--) {
        *psrc++ = c;
    }

    return src;
}