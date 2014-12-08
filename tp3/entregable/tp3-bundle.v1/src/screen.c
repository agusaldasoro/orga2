/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "util.h"
#include "colors.h"

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



void clear_screen() {
    int size = VIDEO_COLS * VIDEO_FILS;    
    ca (*p) = (ca (*)) VIDEO; // magia
    int i = 0;
    ca empty;
    empty.c = 0;
    empty.a = getFormat(C_FG_BLACK, 0, C_BG_BLACK, 0);
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

    clear_screen();
    
    for(y = 1; y < rows; y++) {
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
        for(x = 35; x < 40; x++) {
            screen[y][x] = red;
        }
    }

    for(y = rows-5; y < rows; y++) {
        for(x = 40; x < 45; x++) {
            screen[y][x] = blue;
        }
    }
        /** Escribe a los numeritos de los zombies de cada jugador con sus cruces*/
    char * text;
    text = "1 2 3 4 5 6 7 8 9 10",0;
    print_string(text, 4, 46, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));
    print_string(text, 55, 46, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));

    text = "x x x x x x x x x x",0;
    print_string(text, 4, 48, getFormat(C_FG_RED, 0, C_BG_BLACK  , 0));
    print_string(text, 55, 48, getFormat(C_FG_BLUE, 0, C_BG_BLACK  , 0));

    /** Imprime puntajes */
    text = "00",0;
    print_string(text, 31, 47, getFormat(C_FG_WHITE, 0, C_BG_RED  , 0));
    print_string(text, 48, 47, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));

    text = "0",0;
    print_string(text, 37, 47, getFormat(C_FG_WHITE, 0, C_BG_RED  , 0));
    print_string(text, 42, 47, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));

   // print_debugger(0, 0);
}


void print_exception(int number, registers* regs) {

    clear_screen();

    char * str = exceptions[number];

    print_string(str, 5, 5, 0);

    printf(45, 8, "%s: %h", "eax", regs->eax);
    printf(45, 9, "%s: %h", "ebx", regs->ebx);
    printf(45, 10, "%s: %h", "ecx", regs->ecx);
    printf(45, 11, "%s: %h", "edx", regs->edx);
    printf(45, 12, "%s: %h", "edi", regs->edi);
    printf(45, 13, "%s: %h", "esi", regs->esi);
    printf(45, 14, "%s: %h", "ebp", regs->ebp);
    printf(45, 15, "%s: %h", "esp", regs->esp);


    printf(45, 17, "%s: %h", "cs", regs->cs);
    printf(45, 18, "%s: %h", "ds", regs->ds);
    printf(45, 19, "%s: %h", "es", regs->es);
    printf(45, 20, "%s: %h", "fs", regs->fs);
    printf(45, 21, "%s: %h", "gs", regs->gs);
    printf(45, 22, "%s: %h", "ss", regs->ss);

}

char* getClassName(u8 class) {
    if (class == 0) return "Guerrero";
    if (class == 1) return "Mago";
    if (class == 2) return "Clerigo";

    return "invalid class";
}

void print_debugger(unsigned int player, unsigned char class, registers* registers){

    // TODO: copiar_mapa() en algun espacio de memoria libre
    ca (*screen)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    ca gris;
    gris.c = 0;
    gris.a = getFormat(C_FG_WHITE, 0, C_BG_LIGHT_GREY , 0);

    ca negro;
    negro.c = 0;
    negro.a = getFormat(C_FG_BLACK, 0, C_BG_BLACK , 0);

    ca azul;
    azul.c = 0;
    azul.a = getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0);

    ca rojo;
    rojo.c = 0;
    rojo.a = getFormat(C_FG_WHITE, 0, C_BG_RED , 0);

    int x;
    int y;

//Primera y ultima filas pintadas de negro
    for (x = 25; x < 55; ++x){
        screen[7][x] = negro;
        screen[42][x] = negro;
    }

//Primera y ultima columnas pintadas de negro
    for (y = 7; y < 42; ++y){
        screen[y][25] = negro;
        screen[y][54] = negro;
    }

//Lo del medio de gris
    for (y = 9; y < 42; ++y){
        for (x = 26; x < 54; ++x){
            screen[y][x] = gris;
        }
    }

//La barra de estado de que jugador y zombie es
    for (x = 26; x < 54; ++x){
        screen[8][x] = (player ? azul : rojo);
    }
    u8 attr = getFormat(C_FG_WHITE, 0, (player ? C_BG_BLUE : C_BG_RED), 0);
    print_string((player ? "Zombie B" : "Zombie A"), 26, 8, attr);
    print_string(getClassName(class), 36, 8, attr);

    u8 grey = getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0);

    printfc(27, 10, grey, "eax %h", registers->eax);
    printfc(27, 12, grey, "ebx %h", registers->ebx);
    printfc(27, 14, grey, "ecx %h", registers->ecx);
    printfc(27, 16, grey, "edx %h", registers->edx);
    printfc(27, 18, grey, "esi %h", registers->esi);
    printfc(27, 20, grey, "edi %h", registers->edi);
    printfc(27, 22, grey, "ebp %h", registers->ebp);
    printfc(27, 24, grey, "esp %h", registers->esp);
    // printfc(27,26, grey, "eip %h          cr0", registers->e);

    printfc(42, 10, grey, "cr0 %h", registers->eax);
    printfc(42, 12, grey, "cr1 %h", registers->eax);
    printfc(42, 14, grey, "cr2 %h", registers->eax);
    printfc(42, 16, grey, "cr3 %h", registers->eax);
    printfc(42, 18, grey, "cr4 %h", registers->eax);


    printfc(42, 27, grey, "%s", "Stack");
    printfc(42, 29, grey, "%h", registers->stack0);
    printfc(42, 30, grey, "%h", registers->stack1);
    printfc(42, 31, grey, "%h", registers->stack2);
    printfc(42, 32, grey, "%h", registers->stack3);
    printfc(42, 33, grey, "%h", registers->stack4);
    printfc(42, 34, grey, "%h", registers->stack5);


    printfc(27, 28, grey, "cs %h", registers->cs);
    printfc(27, 30, grey, "ds %h", registers->ds);
    printfc(27, 32, grey, "es %h", registers->es);
    printfc(27, 34, grey, "fs %h", registers->fs);
    printfc(27, 36, grey, "gs %h", registers->gs);
    printfc(27, 38, grey, "ss %h", registers->ss);
    printfc(27, 40, grey, "eflags %h", registers->eax);


/** TODO: Imprimir los valores de los registros

    // pegar_mapa() que teniamos guardado */
}

void toggle_debugger() {
    debugger = !debugger;
    if (debugger) {
        memcpy((void*) 0xb8000, (void*) map, 8000);
    } else {
        memcpy((void*) map, (void*) 0xb8000, 8000);

    }
}