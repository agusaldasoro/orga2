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

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    p[row][col].c = c;
    p[row][col].a = attr;

}

int digits(unsigned int n) {
    int digits = 0;
    while(n >= 10) {
        n = n / 10;
        digits++;
    }

    return digits + 1;
} 

void print_int(int n, unsigned int col, unsigned int row) {
    char numbers[10] = "0123456789";
    int d = digits(n);
    while(n > 0) {
        putc(numbers[n % 10], col+d-1, row);
        n /= 10;
        d--;
    }
}

// Solo voy a imprimir ints y strings.
void printf(unsigned int col, unsigned int row, const char *format, ...) {
    // unsigned char attr = getFormat(C_FG_WHITE, 0, C_BG_BLACK, 0);
    
    va_list args;
    va_start(args, format);

    col--;
    int nextArg = 0;
    int n;
    // unsigned int* p;
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
                    col += digits(n);
                    break;
                case 'h':
                    n = va_arg(args,int);
                    print_string("0x", col, row, 0);
                    print_hex(n, 8, col+2, row, 0);
                    col += digits(n) + 2;
                    break;
                case 'p':
                    n = va_arg(args, unsigned int);
                    print_string("0x",col, row,0);
                    print_hex(n, 8, col+2, row, 0);
                    col += digits(n) + 2;
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
                    // print_string("Unknown format! Stay calm, it's just a bug. AAAAAAA WE ALL GONNA DIE!!!", row, col, 0);
            }
        }
        
        // use defines!
        if (col == 80) {
            col = 0;
            row++;
        }
        nextArg = 0;
        format++;
    }
    va_end(args);


}