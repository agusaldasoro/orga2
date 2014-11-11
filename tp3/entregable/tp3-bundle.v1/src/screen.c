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

// static char *registers[] = {
//     "eax",
//     "ebx",
//     "ecx",
//     "edx",
//     "edi",
//     "esi",
//     "esp",
//     "ebp"
// };


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
    print_string(text, 55, 48, getFormat(C_FG_RED, 0, C_BG_BLACK  , 0));

    /** Imprime puntajes */
    text = "00",0;
    print_string(text, 31, 47, getFormat(C_FG_WHITE, 0, C_BG_RED  , 0));
    print_string(text, 48, 47, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));

    text = "0",0;
    print_string(text, 37, 47, getFormat(C_FG_WHITE, 0, C_BG_RED  , 0));
    print_string(text, 42, 47, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));

    print_debugger(0, 0);
}


// esto no anda.
void print_exception(int number) {

    clear_screen();

    char * str = exceptions[number];
    int tmp;


    print_string(str, 5, 5, 0);

    print_backtrace(5, 8);

    GET_REGISTER("eax", tmp);
    printf(45, 8, "%s: %h", "eax", tmp);

    GET_REGISTER("ebx", tmp);
    printf(45, 9, "%s: %h", "ebx", tmp);

    GET_REGISTER("ecx", tmp);
    printf(45, 10, "%s: %h", "ecx", tmp);

    GET_REGISTER("edx", tmp);
    printf(45, 11, "%s: %h", "edx", tmp);

    GET_REGISTER("edi", tmp);
    printf(45, 12, "%s: %h", "edi", tmp);

    GET_REGISTER("esi", tmp);
    printf(45, 13, "%s: %h", "esi", tmp);

    GET_REGISTER("ebp", tmp);
    printf(45, 14, "%s: %h", "ebp", tmp);

    GET_REGISTER("esp", tmp);
    printf(45, 15, "%s: %h", "esp", tmp);



    // imprir stack!



    // testing.C_BG_LACK
    // row++;
    // putc(7*16+1, col, row); // deberia imprimir "q"
    // row++;
    // print_int(51, col, row); // deberia imprimir "51"
    // row++;
    // putc(48, col, row); // deberia ser una "0"
    // putc(120, col+1, row); // deberia ser una "x"
    // print_hex(100, col+2, row, 0); // deberia imprimir "64"
    // row++;
    // putc(48, col, row); // deberia ser una "0"
    // putc(120, col+1, row); // deberia ser una "x"
    // print_hex((unsigned int) exceptions, col+2, row, 0);
    // row++;
    // char * test = "queso";
    // printf(col, row, "'%s' es un string de %d caracters y esta en memoria %p", test, strlen(test), test);
    // row++;
    // test = 0;
    // printf(col, row, "'%s' es un string de %d caracters y esta en memoria %p", test, strlen(test), test);

}

void print_debugger(unsigned int player, unsigned char class){

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
    if (player){
        for (x = 25; x < 55; ++x){
            screen[8][x] = azul;
        }
        print_string("Zombie B", 26, 8, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));
        if (class == 0)
            print_string("Guerrero", 36, 8, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));
        if (class == 1)
            print_string("Mago", 36, 8, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));
        if (class == 2)
            print_string("Clerigo", 36, 8, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));
    }else{
        for (x = 25; x < 55; ++x){
            screen[8][x] = rojo;
        }
        print_string("Zombie A", 26, 8, getFormat(C_FG_WHITE, 0, C_BG_RED , 0));
        if (class == 0)
            print_string("Guerrero", 36, 8, getFormat(C_FG_WHITE, 0, C_BG_RED , 0));
        if (class == 1)
            print_string("Mago", 36, 8, getFormat(C_FG_WHITE, 0, C_BG_RED , 0));
        if (class == 2)
            print_string("Clerigo", 36, 8, getFormat(C_FG_WHITE, 0, C_BG_RED , 0));
    }

    print_string("eax           cr0", 27, 10, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("ebx           cr2", 27, 12, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("ecx           cr3", 27, 14, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("edx           cr4", 27, 16, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("esi", 27, 18, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("edi", 27, 20, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("ebp", 27, 22, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("esp", 27, 24, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string("eip", 27, 26, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));

    print_string("stack", 41, 27, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));

    print_string(" cs", 27, 28, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string(" ds", 27, 30, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string(" es", 27, 32, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string(" fs", 27, 34, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string(" gs", 27, 36, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string(" ss", 27, 38, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));
    print_string(" eflags", 27, 40, getFormat(C_FG_BLACK, 0, C_BG_LIGHT_GREY , 0));


/** TODO: Imprimir los valores de los registros

    // pegar_mapa() que teniamos guardado */
}