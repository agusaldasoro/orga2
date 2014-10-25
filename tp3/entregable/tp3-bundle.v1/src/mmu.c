/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


void mmu_inicializar() {
	
}

void mmu_inicializar_dir_kernel() {

	page_directory *pd = (page_directory *) 0x27000;

	int i; //,j;

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

// void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
// int munmap(void *addr, size_t length);






