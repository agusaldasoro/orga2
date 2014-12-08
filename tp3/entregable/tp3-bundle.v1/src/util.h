#ifndef __UTIL_H__
#define __UTIL_H__

#include "defines.h"
#include <stdarg.h>

int digits(unsigned int n, int base);

unsigned int strlen(const char *s);
void printf(unsigned int col, unsigned int row, const char *format, ...);
void printfc(unsigned int col, unsigned int row, char attr, const char *format, ...);
void _printf(unsigned int col, unsigned int row, char attr, const char *format, va_list args);
void putc(char c, unsigned int col, unsigned int row);
void print_int(unsigned int number, unsigned int col, unsigned int row);
void print_intb(unsigned int n, int base, unsigned int col, unsigned int row);

void print_stacktrace(unsigned int col, unsigned int row);
void print_backtrace(unsigned int col, unsigned int row);

#define GET_REGISTER(register, output) __asm __volatile("movl %%"register", %0;" :"=r"(output));

void* memcpy(void* src, void* dst, int length);
void* memset(void* src, int c, int n);

void handle_keyboard_interrumption(u8 scancode);

// sizeof(registers= 14*4 + 10*4 = 56 + 40
typedef struct registers {
	int eax;
    int ebx;
    int ecx;
    int edx;
    int edi;
    int esi;
    int ebp;
    int esp;
    int cs;
    int ds;
    int es;
    int fs;
    int gs;
    int ss;
    int stack0;
    int stack1;
    int stack2;
    int stack3;
    int stack4;
    int stack5;
    int stack6;
    int stack7;
    int stack8;
    int stack9;
} __attribute__((__packed__, aligned (4))) registers;

#endif  /* !__UTIL_H__ */
