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
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void clear_screen();

void print_map();

typedef struct t_schar {
    char ch;
    unsigned char fg_color:4;
	unsigned char bright:1;
	unsigned char bg_color:3;
	unsigned char blink:1;
} __attribute__((__packed__)) t_schar;

#endif  /* !__SCREEN_H__ */
