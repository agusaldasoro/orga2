/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8

#define SIZE_W                  78
#define SIZE_H                  44


/* Indices en la gdt */

/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define GDT_IDX_NULL_DESC      0

#define GDT_IDX_DATA_0         8
#define GDT_IDX_DATA_3         9
#define GDT_IDX_CODE_0         0xA
#define GDT_IDX_CODE_3         0xB
#define GDT_IDX_VIDEO          0xC
#define TSS_INICIAL            0xD
#define TSS_IDLE               0xE

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#define ZOMBIE_VIRTUAL         0x8000000
#define PAGE_SIZE              0x1000

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */



#endif  /* !__DEFINES_H__ */
