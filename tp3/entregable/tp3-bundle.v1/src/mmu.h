/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"
#include "util.h"

typedef struct page_directory {
   unsigned char   p:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   i:1;
	unsigned char   ps:1;
	unsigned char   g:1;
    unsigned char   avl:3;
    unsigned int base:20;
} __attribute__((__packed__, aligned (4))) page_directory;

typedef struct page_table {
    unsigned char   p:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   i:1;
	unsigned char   ps:1;
	unsigned char   g:1;
    unsigned char   avl:3;
    unsigned int base:20;
} __attribute__((__packed__, aligned (4))) page_table;


void mmu_inicializar();
void mmu_inicializar_dir_kernel();
page_directory* mmu_inicializar_dir_zombie(unsigned int player, unsigned char class, unsigned int y);
void mmu_mapear_pagina(unsigned int virtual, page_directory* pd, unsigned int fisica, unsigned char rw, unsigned char us);
void copy_code(u32 fisica, page_directory* cr3, u8 class, u8 player);


unsigned int paginas; 

#define PAGES 0x100000

#define PAGE_DIRECTORY_ADDRESS_A 0x100000
#define PAGE_DIRECTORY_ADDRESS_B 0x120000
#define TABLE_PAGE_ADDRESS_A     0x140000
#define TABLE_PAGE_ADDRESS_B     0x160000

#define GUERRRO 0
#define MAGO 1
#define CLERIGO 2

#endif	/* !__MMU_H__ */




