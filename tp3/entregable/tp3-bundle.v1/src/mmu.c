/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"



void mmu_inicializar() {

	page_directory *pd = (page_directory *) 0x27000;

	int i,j;

	for (i = 0; i < 1024; i++) {
		pd[i] = (page_directory) {};		
	}

	for(i = 0; i < 4; i++) {
		pd[i] = (page_directory) {
			.base = 0x28 + i,
			.rw = 0x1,
			.p = 1,
    	};
	}

	for(j = 0; j < 1; j++) {
			page_table* pt = (page_table*) (0x28000 + j*0x1000);
		for(i = 0; i < 1024; i++) {
			pt[i] = (page_table) {
				.base = i+ j*1024,
				.rw = 0x1,
				.p = 1,
	    	};
		}
	}

}







