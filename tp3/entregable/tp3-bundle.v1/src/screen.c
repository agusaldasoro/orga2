/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "util.h"

// http://en.wikipedia.org/wiki/Interrupt_descriptor_table
static char *exceptions[] = {
    "Division by zero",
    "Debugger",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bounds",
    "Invalid Opcode",
    "Coprocessor not available",
    "Double fault",
    "Coprocessor Segment Overrun (386 or earlier only)",
    "Invalid Task State Segment",
    "Segment not present",
    "Stack Fault",
    "General protection fault",
    "Page fault",
    "reserved",
    "Math Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception"
};

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

void print_string(const char * text, unsigned int x, unsigned int y, unsigned short attr) {

    // avoid black print bug
    unsigned char a = (unsigned char) attr;
    if (a == 0) {
        a = getFormat(C_FG_WHITE, 0, C_BG_BLACK, 0);
    }

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = a;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, unsigned int x, unsigned int y, unsigned short attr) {
    print_intb(numero, 16, x, y);
}


// void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {

//     // avoid black print bug
//     unsigned char a = (unsigned char) attr;
//     if (a == 0) {
//         a = getFormat(C_FG_WHITE, 0, C_BG_BLACK, 0);
//     }

//     ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
//     int i;
//     char hexa[8];
//     char letras[16] = "0123456789ABCDEF";
//     hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
//     hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
//     hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
//     hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
//     hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
//     hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
//     hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
//     hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
//     for(i = 0; i < size; i++) {
//         p[y][x + size - i - 1].c = hexa[i];
//         p[y][x + size - i - 1].a = attr;
//     }
// }




void clear_screen() {
    int size = VIDEO_COLS * VIDEO_FILS;    
    ca (*p) = (ca (*)) VIDEO; // magia
    int i = 0;
    ca empty;
    empty.c = 0;
    empty.a = 0; //getFormat(C_FG_BLACK, 0, C_BG_BLACK, 0);
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

void print_exception(int number) {
    clear_screen();

    char * str = exceptions[number];

    // Imprimir stacktrace
    // Imprimir registros
    // Imprimir magia y colores
    // Imprimir unos Doritos
    // 8====D
    unsigned int row = 5;
    unsigned int col = 15;
    print_string(str, col, row, 0);
    row++;
    putc(7*16+1, col, row); // deberia imprimir "q"
    row++;
    print_int(51, col, row); // deberia imprimir "51"
    row++;
    putc(48, col, row); // deberia ser una "0"
    putc(120, col+1, row); // deberia ser una "x"
    print_hex(100, col+2, row, 0); // deberia imprimir "64"
    row++;
    putc(48, col, row); // deberia ser una "0"
    putc(120, col+1, row); // deberia ser una "x"
    print_hex((unsigned int) exceptions, col+2, row, 0);
    row++;
    char * test = "queso";
    printf(col, row, "'%s' es un string de %d caracters y esta en memoria %p", test, strlen(test), test);
    row++;
    test = 0;
    printf(col, row, "'%s' es un string de %d caracters y esta en memoria %p", test, strlen(test), test);

}

