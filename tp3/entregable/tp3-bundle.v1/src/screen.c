/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"



void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}


// ;;  Color:
// ;;      * Bit #: 7 6 5 4 3 2 1 0
// ;;               | | | | | | | |
// ;;               | | | | | ^-^-^-- Fore color
// ;;               | | | | ^-------- Fore color bright bit
// ;;               | ^-^-^---------- Back color
// ;;               ^---------------- Back color bright bit OR enables blinking text
unsigned char getFormat(unsigned char fore_color, char fore_bright, unsigned char back_color, char blink) {
    return fore_color | fore_bright | back_color | blink; 
}

void clear_screen() {
    int size = VIDEO_COLS * VIDEO_FILS;    
    ca (*p) = (ca (*)) VIDEO; // magia
    int i = 0;
    ca empty;
    empty.c = 71;
    empty.a = 71; //getFormat(C_FG_BLACK, 0, C_BG_BLACK, 0);
    while(i < size) {
        p[i] = empty;
        i++;
    }
}

void print_map() {
    int cols = VIDEO_COLS;
    int rows = VIDEO_FILS;

    ca (*screen)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

    ca red;
    red.c = 0;
    red.a = getFormat(C_FG_RED, 0, C_BG_RED, 0);
    ca blue;
    blue.c = 0;
    blue.a = getFormat(C_FG_BLUE, 0, C_BG_BLUE, 0);
    ca green;
    green.c = 0;
    green.a = getFormat(C_FG_GREEN, 0, C_BG_GREEN, 0);
    ca black;
    black.c = 0;
    black.a = getFormat(C_FG_BLACK, 0, C_BG_BLACK, 0);

    int y,x;
    
    for(y = 0; y < rows; y++) {
        for(x = 0; x < cols; x++) {
            if (y >= rows-5) {
                screen[y][x] = black;
            } else if (x == cols-1) {
                screen[y][x] = blue;
            } else if (x == 0) {
                screen[y][x] = red;
            } else {
                screen[y][x] = green;
            }
        }
    }
    for(y = rows-5; y < rows; y++) {
        for(x = 0; x < cols; x++) {
        }
    }


}