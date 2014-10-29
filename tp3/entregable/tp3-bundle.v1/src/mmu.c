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
	pd += paginas*4096;
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

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica) {
    page_directory *pd = (page_directory *) cr3;
    page_table* pt;

    unsigned int offset_dir = virtual >> 12;
    unsigned int offset_table = (virtual >> 12) - offset_dir;
    offset_dir = offset_dir >> 10;

    unsigned int segmento_fisica = fisica >> 12;

    if (pd[offset_dir].p == 0){
        pd[offset_dir].base = ((unsigned int) dame_pt()) >> 12;
        pd[offset_dir].rw = 1;
        pd[offset_dir].p = 1;
    }

    pt = (page_table*) (pd[offset_dir].base << 12);
    pt[offset_table].base = segmento_fisica;
    pt[offset_table].rw = 1;
    pt[offset_table].p = 1;
    tlbflush();
}

void mmu_inicializar_dir_zombie(unsigned int player, unsigned int y) {

	unsigned int pd = (unsigned int) dame_pd();

	unsigned int x = (player ? 79 : 2);
// player = 0 es A
// player = 1 es B
	if (player) {
		//player B
		mmu_mapear_pagina(0x8000000, pd, get_physical_address(x, y));
		mmu_mapear_pagina(0x8001000, pd, get_physical_address(x-1, y));
		mmu_mapear_pagina(0x8002000, pd, get_physical_address(x-1, y-1));
		mmu_mapear_pagina(0x8003000, pd, get_physical_address(x-1, y+1));
		mmu_mapear_pagina(0x8004000, pd, get_physical_address(x, y-1));
		mmu_mapear_pagina(0x8005000, pd, get_physical_address(x, y+1));
		mmu_mapear_pagina(0x8006000, pd, get_physical_address(x+1, y));
		mmu_mapear_pagina(0x8007000, pd, get_physical_address(x+1, y+1));
		mmu_mapear_pagina(0x8008000, pd, get_physical_address(x+1, y-1));
	} else {
		//player A
		mmu_mapear_pagina(0x8000000, pd, get_physical_address(x, y));
		mmu_mapear_pagina(0x8001000, pd, get_physical_address(x+1, y));
		mmu_mapear_pagina(0x8002000, pd, get_physical_address(x+1, y+1));
		mmu_mapear_pagina(0x8003000, pd, get_physical_address(x+1,y -1));
		mmu_mapear_pagina(0x8004000, pd, get_physical_address(x, y+1));
		mmu_mapear_pagina(0x8005000, pd, get_physical_address(x, y-1));
		mmu_mapear_pagina(0x8006000, pd, get_physical_address(x-1, y));
		mmu_mapear_pagina(0x8007000, pd, get_physical_address(x-1, y-1));
		mmu_mapear_pagina(0x8008000, pd, get_physical_address(x-1, y+1));

	}


// 	if (player == 1) {
// //		pd = (page_directory *) PAGE_DIRECTORY_ADDRESS_A;
// 	} else {
// //		pd = (page_directory *) PAGE_DIRECTORY_ADDRESS_B;
		
// 	}

// 	int i;

// 	/// Creo 1024 entradas en page_directory con todo cero.
// 	for (i = 0; i < 1024; i++) {
// 		pd[i] = (page_directory) {};		
// 	}
// 	pd[0] = (page_directory) {
// 		.base = 0x28 + i,
// 		.rw = 0x1,
// 		.p = 1,
// 	};

}






