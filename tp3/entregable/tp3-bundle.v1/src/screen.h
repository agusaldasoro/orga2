/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80



#include "colors.h"
#include "defines.h"
#include "util.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

void print_string(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, unsigned int x, unsigned int y, unsigned short attr);

unsigned char getFormat(unsigned char fore_color, char fore_bright, unsigned char back_color, char blink);

void clear_screen();
void print_map();
void print_exception(int number, registers* regs);
void print_debugger(unsigned int player, unsigned char class);

#endif  /* !__SCREEN_H__ */
