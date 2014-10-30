/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


void mmu_inicializar_dir_kernel() {
	page_directory *pd = (page_directory *) 0x27000;

	int i;

	/// Creo 1024 entradas en page_directory con todo cero.
	for (i = 0; i < 1024; i++) {
		pd[i] = (page_directory) {};
	}


	// Mapeo las primeras 4 (0,1,2,3) entradas del page direcotry
	// con el base y permisos correspondientes.
	for(i = 0; i < 4; i++) {
		pd[i] = (page_directory) {
			.base = 0x28 + i,
			.rw = 0x1,
			.p = 1,
    	};
	}

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
	mmu_inicializar_dir_zombie(1, 3);
}

page_directory* dame_pd() {
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

/**
Bastante sugestivo, no?
**/
page_table* dame_pt() {
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
	unsigned int ret;

	if (y==-1) {
		get_physical_address(x, 44);
	} else if (y==45) {
		get_physical_address(x, 0);
	} else {
		ret = (x + y*50) * 0x1000;
	}

	ret = ret + 0x400000;
	return ret;
}

void mmu_mapear_pagina(unsigned int virtual, page_directory* pd, unsigned int fisica, unsigned char rw, unsigned char us) {


    unsigned int directory = (virtual >> 22);
    unsigned int table     = (virtual & 0x003FF000) >> 12;

    page_table* pt = (page_table*) (pd[directory].base << 12);

    if (pd[directory].p == 0){
        pd[directory].base = ((unsigned int) dame_pt()) >> 12;
        pd[directory].rw = rw;
	    pd[directory].us = us;
        pd[directory].p = 1;
    }

    pt = (page_table*) (pd[directory].base << 12);
    pt[table].base = fisica >> 12;
    pt[table].rw = rw;
    pt[table].us = us;
    pt[table].p = 1;

    tlbflush();
}

void mmu_inicializar_dir_zombie(unsigned int player, unsigned int y) {

	page_directory* pd = dame_pd();

	unsigned int x = (player ? 79 : 2);
// player = 0 es A
// player = 1 es B
	if (player) {
		//player B
		mmu_mapear_pagina(0x8000000, pd, get_physical_address(x, y), 1, 0);
		mmu_mapear_pagina(0x8001000, pd, get_physical_address(x-1, y), 1, 0);
		mmu_mapear_pagina(0x8002000, pd, get_physical_address(x-1, y-1), 1, 0);
		mmu_mapear_pagina(0x8003000, pd, get_physical_address(x-1, y+1), 1, 0);
		mmu_mapear_pagina(0x8004000, pd, get_physical_address(x, y-1), 1, 0);
		mmu_mapear_pagina(0x8005000, pd, get_physical_address(x, y+1), 1, 0);
		mmu_mapear_pagina(0x8006000, pd, get_physical_address(x+1, y), 1, 0);
		mmu_mapear_pagina(0x8007000, pd, get_physical_address(x+1, y+1), 1, 0);
		mmu_mapear_pagina(0x8008000, pd, get_physical_address(x+1, y-1), 1, 0);
	} else {
		//player A
		mmu_mapear_pagina(0x8000000, pd, get_physical_address(x, y), 1, 0);
		mmu_mapear_pagina(0x8001000, pd, get_physical_address(x+1, y), 1, 0);
		mmu_mapear_pagina(0x8002000, pd, get_physical_address(x+1, y+1), 1, 0);
		mmu_mapear_pagina(0x8003000, pd, get_physical_address(x+1,y -1), 1, 0);
		mmu_mapear_pagina(0x8004000, pd, get_physical_address(x, y+1), 1, 0);
		mmu_mapear_pagina(0x8005000, pd, get_physical_address(x, y-1), 1, 0);
		mmu_mapear_pagina(0x8006000, pd, get_physical_address(x-1, y), 1, 0);
		mmu_mapear_pagina(0x8007000, pd, get_physical_address(x-1, y-1), 1, 0);
		mmu_mapear_pagina(0x8008000, pd, get_physical_address(x-1, y+1), 1, 0);

	}
}

unsigned int recuperar_fisica(unsigned int virtual, unsigned int cr3){
    page_directory* pd = (page_directory*) cr3;
    page_table* pt = (page_table*) (pd[virtual >> 22].base << 12);
    unsigned int table  = (virtual & 0x003FF000) >> 12;
    unsigned int res = (unsigned int) (pt[table].base << 12);
    return res;
}

void desplazar_fisica(unsigned int virtual, unsigned int cr3, int x, int y){
    page_directory* pd = (page_directory*) cr3;
    page_table* pt = (page_table*) (pd[virtual >> 22].base << 12);
    unsigned int table  = (virtual & 0x003FF000) >> 12;
    pt[table].base += x;
    pt[table].base += y*50;
}

void mover_soldado(int x, int y,unsigned int cr3){
    int i = 0;
    while(i<9){
        desplazar_fisica(0x8000000+i*0x1000,cr3,x,y);
        i++;
    }
}






