/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

void tss_inicializar() {

	// inicializar tss_idle
	tss_inicializar_tarea_idle();

    memcpy(&tss_idle, &tss_inicial, sizeof(tss));

    add_entry((unsigned int) &tss_inicial, 0x67, 0x9, 1, 0);
    add_entry((unsigned int) &tss_idle, 0x67, 0x9, 1, 0);
}

void tss_inicializar_tarea_idle() {

	tss_idle = (tss) {};

	tss_idle.eip = 0x00016000;
	tss_idle.cr3 = 0x27000;
	
	tss_idle.ebp = 0x27000;
	tss_idle.esp = 0x27000;

	tss_idle.es = 0x40;
    tss_idle.ds = 0x40;
    tss_idle.ss = 0x40;   
    tss_idle.gs = 0x40;
    tss_idle.cs = 0x50;

	tss_idle.eflags = 0x202;
}


void init_tss(tss* tss, u32 cr3, u32 eip, u32 stack, u16 ds, u16 cs, u32 eflags) {
	tss = (tss) {};

	tss.cr3 = cr3;
	tss.eip = cr3;
	
	tss.ebp = stack;
	tss.esp = stack;


	tss.ds = ds;
	tss.es = ds;
	tss.ss = ds;
	tss.gs = ds;

	tss.cs = cs;

	
	tss.eflags = eflags;
	tss.cr3 = cr3;

}