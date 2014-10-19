#ifndef __UTIL_H__
#define __UTIL_H__

// #include "defines.h"

int digits(unsigned int n, int base);

unsigned int strlen(const char *s);
void printf(unsigned int col, unsigned int row, const char *format, ...);
void putc(char c, unsigned int col, unsigned int row);
void print_int(int number, unsigned int col, unsigned int row);
void print_intb(int n, int base, unsigned int col, unsigned int row);

#endif  /* !__UTIL_H__ */
