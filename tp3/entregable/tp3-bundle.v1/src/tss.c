/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "i386.h"
#include "colors.h"
#include "mmu.h"
#include "screen.h"

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];
//char noEstoyEnLaIdl;

u8 inUseA[CANT_ZOMBIS] = {};
u8 inUseB[CANT_ZOMBIS] = {};

u8 tiposA[CANT_ZOMBIS] = {};
u8 tiposB[CANT_ZOMBIS] = {};

unsigned int clock_de_zombiesA [CANT_ZOMBIS] = {};
unsigned int clock_de_zombiesB [CANT_ZOMBIS] = {};

int currentZombieA;
int currentZombieB;

u8 currentPlayer;

u8 current_selector;

void tss_inicializar() {
	//noEstoyEnLaIdl = 1;
	int i = 0;
	while(i < CANT_ZOMBIS) {
		inUseA[i] = 0;
		inUseB[i] = 0;
		i++;
	}
	currentZombieA = 0;
	currentZombieB = 0;

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

/*
	gdt[GDT_NEXT_TSS].base_31_24 = ((u32) (&next_task) & 0xFF000000) >> 24;
	gdt[GDT_NEXT_TSS].base_23_16 = ((u32) (&next_task) & 0x00FF0000) >> 16;
	gdt[GDT_NEXT_TSS].base_0_15  = (u32) (&next_task) & 0x0000FFFF;
*/
    // add_entry((unsigned int) &tss_inicial, 0x67, 0x9, 1, 0);
    // add_entry((unsigned int) &tss_idle, 0x67, 0x9, 1, 0);
}

void tss_inicializar_tarea_idle() {

	gdt[GDT_TSS_IDLE].base_0_15 = ((unsigned int)&tss_idle) & 0x0000FFFF;
	gdt[GDT_TSS_IDLE].base_23_16 = (((unsigned int)&tss_idle) & 0x00FF0000) >> 16;
	gdt[GDT_TSS_IDLE].base_31_24 = (((unsigned int)&tss_idle) & 0xFF000000) >> 24;

	tss_idle = (tss) {};

	tss_idle.eip = 0x00016000;
	tss_idle.cr3 = rcr3();
	
	tss_idle.ebp = 0x27000;
	tss_idle.esp = 0x27000;

	tss_idle.es = 0x40;
    tss_idle.ds = 0x40;
    tss_idle.ss = 0x40;
    tss_idle.gs = 0x40;
    tss_idle.cs = 0x50;

	tss_idle.eflags = 0x202;
	tss_idle.iomap = 0xffff;

	tss_idle.esp0 = 0x27000;
	tss_idle.ss0 = 0x40;
}


tss* _get_next_tss(u8 player) {
	int i = 0;
	tss* ret = 0;

	//char* text;
	//text = "me declaro spectrum",0;
	
	if (player) {
		i = currentZombieB;
		do {
		 	i++;
			currentZombieB++;
			currentZombieB = currentZombieB % 8;
		} while(!inUseB[currentZombieB] && i <= CANT_ZOMBIS);

		if (inUseB[currentZombieB]){
			ret = &tss_zombisB[currentZombieB];
			claseActual = tiposB[currentZombieB];
			clock_de_zombiesB[currentZombieB]++;
			print_clock_zombie(1,currentZombieB,clock_de_zombiesB[currentZombieB]);

		}

	} else {
		i = currentZombieA;
		do {
		 	i++;
			currentZombieA++;
			currentZombieA = currentZombieA % 8;
		} while(!inUseA[currentZombieA] && i <= CANT_ZOMBIS);

		if (inUseA[currentZombieA]){
			ret = &tss_zombisA[currentZombieA];
			claseActual = tiposA[currentZombieA];
			clock_de_zombiesA[currentZombieA]++;
			print_clock_zombie(0,currentZombieA,clock_de_zombiesA[currentZombieA]);
		}
	}

	return ret;
}

tss* get_next_tss() {

	//breakpoint();

	currentPlayer = !currentPlayer;

	tss * ret = _get_next_tss(currentPlayer);

	// No hay player en el otro jugador? No problem, agarramos el próximo tss del jugador actual.
	if (!ret) {
		
		currentPlayer = !currentPlayer;
		ret = _get_next_tss(currentPlayer);
	} 

	if (!ret) {
		//char* text;
		//text = "no encuentra tss",0;
		//print_string(text, 22, 22, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));

		ret = &tss_idle;
		//ret = 0;
	}

	return ret;
}



tss* get_free_tss(u8 player,u8 class) {
	int i = 0;

	if (player) {
		while(i<CANT_ZOMBIS && inUseB[i]) i++;
	} else {
		while(i<CANT_ZOMBIS && inUseA[i]) i++;

	}

	if(i==CANT_ZOMBIS) return 0;

	if(player){
		inUseB[i] = 1;
		tiposB[i] = class;
		clock_de_zombiesB[i] = 0;
	}else{
		inUseA[i] = 1;
		tiposA[i] = class;
		clock_de_zombiesA[i] = 0;
	}

	return (player ? &tss_zombisB[i] : &tss_zombisA[i]);

}

u8 is_busy(gdt_entry* tss_selector) {
	//print_hex(currentZombieB,34,34,getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));
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

	char* text;
	text = "me declaro spectrum",0;

	// El proximo es el que no está busy.
	int next_selector = is_busy(&gdt[GDT_CURRENT_TSS]) ? GDT_NEXT_TSS : GDT_CURRENT_TSS;
	if(is_busy(&gdt[next_selector]))print_string(text, 22, 22, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));		

	tss* next_tss = get_next_tss();

	if(next_tss==&tss_idle || termino_el_juego){
		if(is_busy(&gdt[GDT_TSS_IDLE])){
			return 0;
		}else{
			return 0x80;
		}
	}

	// Si es el proximo tss es igual al ya seteado devolvemos 0.
	if ((u32) &next_tss == tss_get_base(&gdt[next_selector]) && !is_busy(&gdt[GDT_TSS_IDLE]) ) {
		return 0;
	}

	tss_set_base(&(gdt[next_selector]), (u32) next_tss);
	current_selector = next_selector;
	//noEstoyEnLaIdl = 1;
	return next_selector * 8;
}

tss* recuperarBase(unsigned int indice){
	return (tss*)(gdt[indice].base_0_15 + (gdt[indice].base_23_16 << 16) + (gdt[indice].base_31_24 << 24));
}

u8 desalojarTarea(){
	tss* aDesalojar;
	if(is_busy(&gdt[GDT_NEXT_TSS])){
		aDesalojar = recuperarBase(GDT_NEXT_TSS);
	}else{
		aDesalojar = recuperarBase(GDT_CURRENT_TSS);
	}
	int i = 0;
	while(i< CANT_ZOMBIS && aDesalojar != &(tss_zombisA[i]))i++;
	if(i<CANT_ZOMBIS){
		inUseA[i] = 0;
		clock_de_zombiesA[i] = 0;
		reset_clock_zombie(0,i);
		return 0;
	}else{
		i = 0;
		while(i< CANT_ZOMBIS && aDesalojar != &(tss_zombisB[i]))i++;
		if(i<CANT_ZOMBIS){
			inUseB[i] = 0;
			clock_de_zombiesB[i] = 0;
			reset_clock_zombie(1,i);
			return 1;
		}
	}
	return 2;
}


void init_tss(tss* tss, u32 cr3, u32 eip, u32 stack, u16 ds, u16 cs, u32 eflags) {

	memset(tss, 0, sizeof(tss));
	breakpoint();

	tss->cr3 = cr3;
	tss->eip = 0x8000000;
	
	tss->ebp = stack;
	tss->esp = stack;


	tss->ds = ds;
	tss->es = ds;
	tss->ss = ds;
	tss->gs = ds;
	tss->cs = cs;
	tss->fs = 0x60;
	tss->eax = 0x3498;
	
	tss->eflags = eflags;
	tss->iomap = 0xffff;
	tss->esp0 = 0x300000 - (paginas * 0x1000);
	//tss->esp0 = (unsigned int) get_page_table() + 0x1000;
	tss->ss0 = 0x40;
	tss->eip = 0x8000000;
}
