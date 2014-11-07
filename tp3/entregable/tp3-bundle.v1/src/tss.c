/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"


tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

u8 inUseA[CANT_ZOMBIS] = {};
u8 inUseB[CANT_ZOMBIS] = {};

int currentZombieA;
int currentZombieB;

u8 currentPlayer_rait_nau;

u8 current_selector;

void tss_inicializar() {

	// inicializar tss_idle
	tss_inicializar_tarea_idle();

    memcpy(&tss_idle, &tss_inicial, sizeof(tss));
    memcpy(&tss_idle, &current_task, sizeof(tss));
    memcpy(&tss_idle, &next_task, sizeof(tss));

	gdt[GDT_INITIAL_TSS].base_31_24 = ((u32) (&tss_inicial) & 0xFF000000) >> 24;
	gdt[GDT_INITIAL_TSS].base_23_16 = ((u32) (&tss_inicial) & 0x00FF0000) >> 16;
	gdt[GDT_INITIAL_TSS].base_0_15  = (u32) (&tss_inicial) & 0x0000FFFF;

	gdt[GDT_CURRENT_TSS].base_31_24 = ((u32) (&current_task) & 0xFF000000) >> 24;
	gdt[GDT_CURRENT_TSS].base_23_16 = ((u32) (&current_task) & 0x00FF0000) >> 16;
	gdt[GDT_CURRENT_TSS].base_0_15  = (u32) (&current_task) & 0x0000FFFF;

	gdt[GDT_NEXT_TSS].base_31_24 = ((u32) (&next_task) & 0xFF000000) >> 24;
	gdt[GDT_NEXT_TSS].base_23_16 = ((u32) (&next_task) & 0x00FF0000) >> 16;
	gdt[GDT_NEXT_TSS].base_0_15  = (u32) (&next_task) & 0x0000FFFF;

    // add_entry((unsigned int) &tss_inicial, 0x67, 0x9, 1, 0);
    // add_entry((unsigned int) &tss_idle, 0x67, 0x9, 1, 0);
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
	tss_idle.iomap = 0xffff;
}


/**
Lafunction maestra.
**/

tss* _get_next_tss(u8 player) {
	int i = 0;
	tss* ret = 0;
	if (player) {
		do {
		 	i++;
			currentZombieB++;
			currentZombieB = currentZombieB % 8;
		} while(!inUseB[currentZombieB] && i < 8);

		if (inUseB[currentZombieB]) ret = &tss_zombisB[currentZombieB];		

	} else {

		do {
		 	i++;
			currentZombieA++;
			currentZombieA = currentZombieA % 8;
		} while(!inUseA[currentZombieA] && i < 8);

		ret = &tss_zombisA[currentZombieA];

		if (inUseA[currentZombieA]) ret = &tss_zombisA[currentZombieA];		

	}

	return ret;
}

tss* get_next_tss() {

	currentPlayer_rait_nau = !currentPlayer_rait_nau;

	tss * ret = _get_next_tss(currentPlayer_rait_nau);

	// No hay player en el otro jugador? No problem, agarramos el próximo tss del jugador actual.
	if (!ret) {
		currentPlayer_rait_nau = !currentPlayer_rait_nau;
		ret = _get_next_tss(currentPlayer_rait_nau);
	} 
	if (!ret)  {
		ret = &tss_idle;
	}

	return ret;
}



tss* get_free_tss(u8 player) {
	int i = 0;

	if (player) {
		while(inUseB[i]) i++;
	} else {
		while(inUseA[i]) i++;

	}

	return (player ? &tss_zombisB[i] : &tss_zombisA[i]);

}

u8 is_busy(gdt_entry* tss_selector) {
	return (tss_selector->type == 11);
}

void tss_set_base(gdt_entry* sel, u32 base) {
	sel->base_31_24 = (base & 0xFF000000) >> 24;
	sel->base_23_16 = (base & 0x00FF0000) >> 16;
	sel->base_0_15  = base & 0x0000FFFF;
}

u32 tss_get_base(gdt_entry* sel) {
	return (sel->base_31_24 << 24 | sel->base_23_16 << 16 | sel->base_0_15); 
}

int proximo_indice() {
	int next_selector;

	if (is_busy(&gdt[GDT_CURRENT_TSS])) {
		next_selector = GDT_NEXT_TSS;
	} else {
		next_selector = GDT_CURRENT_TSS;
	}

	tss* next_tss = get_next_tss();

	if ((u32) &next_tss == tss_get_base(&gdt[next_selector])) {
		return 0;
	}

	tss_set_base(&gdt[next_selector], (u32) next_tss);
	current_selector = next_selector;
	return next_selector;
}


void init_tss(tss* tss, u32 cr3, u32 eip, u32 stack, u16 ds, u16 cs, u32 eflags) {
	// TODO : Hay que ponerlo en 0, memset?

	tss->cr3 = cr3;
	tss->eip = eip;
	
	tss->ebp = stack;
	tss->esp = stack;


	tss->ds = ds;
	tss->es = ds;
	tss->ss = ds;
	tss->gs = ds;
	tss->cs = cs;
	
	tss->eflags = eflags;


    add_entry((unsigned int) &tss, 0x67, 0x9, 1, 0);

}
