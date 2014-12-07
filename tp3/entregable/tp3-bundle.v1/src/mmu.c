/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#define ANCHO_MAPA 78

void mmu_inicializar_dir_kernel() {
	page_directory *pd = (page_directory *) 0x27000;

	int i;

	/// Creo 1024 entradas en page_directory con todo cero.
	for (i = 0; i < 1024; i++) {
		pd[i] = (page_directory) {};
	}


	// Mapeo solo la primera entrada del page direcotry
	// con la base y permisos correspondientes.

		pd[0] = (page_directory) {
			.base = 0x28,
			.rw = 0x1,
			.p = 1,
    	};

	page_table* pt = (page_table*) 0x28000;
	for(i = 0; i < 1024; i++) {
		pt[i] = (page_table) {
			.base = i, //+ j*1024,
			.rw = 0x1,
			.p = 1,
    	};
	}

}

void mmu_inicializar() {
	mmu_inicializar_dir_kernel();
	paginas = 0;
	//mmu_inicializar_dir_zombie(1, 3, 0);
}

page_directory* get_page_directory() {
	page_directory* pd = (page_directory*) PAGES;
	pd += paginas * 4096;
	int i;
	for (i = 0; i < 1024; i++) {
		pd[i] = (page_directory) {};
	}

	pd[0] = (page_directory) {
		.base = 0x28,
		.rw = 0x1,
		.p = 1
	};

	paginas++;

	return pd;
}


page_table* get_page_table() {
	page_table* pd = (page_table*) PAGES;
	pd += paginas * 0x1000;
	int i;
	for (i = 0; i < 1024; i++) {
		pd[i] = (page_table) {};
	}

	paginas++;

	return pd;
}


unsigned int get_physical_address(unsigned int x, unsigned int y) {

	if (y==-1) {
		return get_physical_address(x, 44);
	} else if (y==45) {
		return get_physical_address(x, 0);
	} else {
	    unsigned int ret;
		ret = (x + y*ANCHO_MAPA) * 0x1000;
		ret = ret + 0x400000;
		return ret;
	}
}

void get_position(unsigned int* x, unsigned int* y,unsigned int dir){
	int y2 = 0;
	dir = dir-0x400000;
	dir = dir/0x1000;
	while(dir<ANCHO_MAPA){
		dir = dir-ANCHO_MAPA;
		y2++;
	}
	*y = y2;
	*x = dir;
}

void mmu_mapear_pagina(unsigned int virtual, page_directory* pd, unsigned int fisica, unsigned char rw, unsigned char us) {

    unsigned int directory = (virtual >> 22);
    unsigned int table     = (virtual & 0x003FF000) >> 12;

    if (pd[directory].p == 0){
        pd[directory].base = ((unsigned int) get_page_table()) >> 12;
        pd[directory].rw = rw;
	    pd[directory].us = us;
        pd[directory].p = 1;
    }

    page_table* pt = (page_table*) (pd[directory].base << 12);
    pt[table].base = fisica >> 12;
    pt[table].rw = rw;
    pt[table].us = us;
    pt[table].p = 1;


    tlbflush();
}


 void mmu_unmapear_pagina(unsigned int virtual,page_directory* cr3){
 	unsigned int directory = (virtual >> 22);
    unsigned int table     = (virtual & 0x003FF000) >> 12;

    page_table* pt = (page_table*) (cr3[directory].base << 12);

    if (cr3[directory].p != 0){
        pt = (page_table*) (cr3[directory].base << 12);
        pt[table].p = 0;
    }

    tlbflush();
 }


/**
	Guerrero = 0
	Mago = 1
	Clerigo = 2
*/
page_directory* mmu_inicializar_dir_zombie(unsigned int player, unsigned char class, unsigned int y) {
	
	page_directory* pd = get_page_directory();
	breakpoint();

	unsigned int x = (player ? 79 : 2);
// player = 0 es A
// player = 1 es B

	unsigned int offset_x[9] = {0, -1, -1, -1, 0, 0, 1, 1, 1};
	unsigned int offset_y[9] = {0, 0, -1, 1, -1, 1, 0, 1, -1};

	int i, _x, _y;
	for(i = 0; i < 9; i++) {
		_x = y + offset_x[i] * (player ? 1 : -1);
		_y = x + offset_y[i] * (player ? 1 : -1);

		mmu_mapear_pagina(0x8000000 + (i*0x1000), pd, get_physical_address(_x, _y), 1, 1);
	}


	//unsigned int address = 0x10000 + (player ? 0 : 1)  * 0x3000 + class * 0x1000;
	breakpoint();
	copy_code(get_physical_address(x,y),(page_directory*)rcr3(),class,player);
	breakpoint();
	//breakpoint();
	//i = 0;
	//unsigned char *code = (unsigned char *) 0x8000000;
	//unsigned char * paddress = (unsigned char*) address;
	//while (i++ < 0x1000) {
	//	code[i] = paddress[i];
	//}


	//lcr3((unsigned int)pd);
	//breakpoint();

	return pd;
}

unsigned int recuperar_fisica(unsigned int virtual, page_directory* pd) {
    page_table* pt = (page_table*) (pd[virtual >> 22].base << 12);
    unsigned int table  = (virtual & 0x003FF000) >> 12;
    return (pt[table].base << 12);
}

void desplazar_fisica(unsigned int virtual, page_directory* pd, int x, int y) {
    page_table* pt = (page_table*) (pd[virtual >> 22].base << 12);
    unsigned int table  = (virtual & 0x003FF000) >> 12;
    pt[table].base += x;
    pt[table].base += y*ANCHO_MAPA;
    tlbflush();
}


void copy_code(u32 fisica, page_directory* cr3, u8 class, u8 player) {
	breakpoint();
	mmu_mapear_pagina(0x4000000, cr3, fisica, 1, 0);
	breakpoint();
	memcpy((void*) 0x10000 + ((player ? 0 : 1)  * 0x3000 + class * 0x1000), (void*) 0x4000000, 0x1000);
	breakpoint();
	mmu_unmapear_pagina(0x4000000, cr3);
}






