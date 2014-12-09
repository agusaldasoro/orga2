/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "sched.h"
#include "screen.h"
#include "mmu.h"

#define ANCHO_MAPA 78
#define ALTO_MAPA 44

#define ZOMBIE_MUERTO 5
#define LIMITE_DE_TIEMPO_INACTIVIDAD 30

u8 puntajeA;
u8 puntajeB;

u8 cantZombiesA;
u8 cantZombiesB;

u8 zombiesActivosA;
u8 zombiesActivosB;

void convertir_a_string_de_dos_digitos(unsigned int n, char* res) {
    unsigned int decena;
    unsigned int unidadades;
    n = n % 100;

    decena = n/10;
    unidadades = n%10;
    res[1] = unidadades + '0';
    res[0] = decena + '0';
}


void game_init() {
	puntajeA = 0;
	puntajeB = 0;
	currentPosA = 20;
	currentPosB = 20;
	zombieClassA = MAGO;
	zombieClassB = MAGO;
	mostrar_cursores(1,0);
	mostrar_cursores(0,0);

	cantZombiesA = 20;
	cantZombiesB = 20;

	reimprimirCantidadZombies(0);
	reimprimirCantidadZombies(1);

	zombiesActivosA = 0;
	zombiesActivosB = 0;
	debugger = 0;

	termino_el_juego = 0;
	contadorDeInactividad = 0;
}

void mostrar_cursores(u8 player,s8 d){
	if(!player){
		print_string(" ",0,currentPosA+1, getFormat(C_FG_WHITE, 0, C_BG_RED, 0));
		print_string(get_zombie_type(zombieClassA),0,currentPosA+d+1, getFormat(C_FG_WHITE, 0, C_BG_RED, 0));
	}else{
		print_string(" ",79,currentPosB+1, getFormat(C_FG_WHITE, 0, C_BG_BLUE, 0));
		print_string(get_zombie_type(zombieClassB),79,currentPosB+d+1, getFormat(C_FG_WHITE, 0, C_BG_BLUE, 0));
	}
}

void reimprimirCantidadZombies(u8 player){
	char* text;
	text = "00",0;
	if(player){
		convertir_a_string_de_dos_digitos(cantZombiesB,text);
		print_string(text, 48, 47, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));
	}else{
		convertir_a_string_de_dos_digitos(cantZombiesA,text);
		print_string(text, 31, 47, getFormat(C_FG_WHITE, 0, C_BG_RED  , 0));
	}   
}

void reimprimirPuntaje(u8 player){
	char* text;
	text = "00",0;
	if(player){
		convertir_a_string_de_dos_digitos(puntajeB,text);
		print_string(text, 42, 47, getFormat(C_FG_WHITE, 0, C_BG_BLUE , 0));
	}else{
		convertir_a_string_de_dos_digitos(puntajeA,text);
		print_string(text, 37, 47, getFormat(C_FG_WHITE, 0, C_BG_RED  , 0));
	}
}

void sumarPunto(u8 player){
	if(player){
		puntajeB++;
	}else{
		puntajeA++;
	}
	reimprimirPuntaje(player);
	u8 zombie_player = desalojarTarea();
	reducirZombiesActivos(zombie_player);
}

void reducirZombiesActivos(u8 player){
	if(player==1){
		zombiesActivosB--;
	}else if(player==0){
		zombiesActivosA--;
	}
}

void destruir_zombie(){
	u8 zombie_player = desalojarTarea();
	reducirZombiesActivos(zombie_player);
}

char* get_zombie_type(u32 type) {
	if (type == 0) return "G";
	if (type == 1) return "M";
	if (type == 2) return "C";
	return "T";
}

void game_change_class(u8 player, s8 d) {
	if (player) {
		zombieClassB = (u8) ((zombieClassB + d + 3) % 3);
		printfc(79, currentPosB+1, getFormat(C_FG_WHITE, 0,C_BG_BLUE,0), "%s", get_zombie_type(zombieClassB));
	} else {
		zombieClassA = (u8) ((zombieClassA + d + 3) % 3);
		printfc(0, currentPosA+1, getFormat(C_FG_WHITE,0,C_BG_RED,0), "%s", get_zombie_type(zombieClassA));
	}
}

void game_move_zombie(u8 player, s8 d) {
	if (player) {
		if (currentPosB+d < 0 || currentPosB+d > ALTO_MAPA-1) return;
		mostrar_cursores(player, d);
		currentPosB = currentPosB + d;
	} else {
		if (currentPosA+d < 0 || currentPosA+d > ALTO_MAPA-1) return;
		mostrar_cursores(player, d);
		currentPosA = currentPosA + d;
	}
}

void game_lanzar_zombi(u8 player) {
	breakpoint();
	if (player) {
		if(cantZombiesB >0 && zombiesActivosB < 8){
			cantZombiesB--;
			zombiesActivosB++;
			reimprimirCantidadZombies(player);
			start_zombie(player, zombieClassB, currentPosB);
		}
	} else {
		if(cantZombiesA > 0 && zombiesActivosA < 8){
			cantZombiesA--;
			zombiesActivosA++;
			reimprimirCantidadZombies(player);
			start_zombie(player, zombieClassA, currentPosA);
		}
	}

}

void mover_soldado(int x2, int y2,unsigned int player, page_directory* pd) {
	breakpoint();
	setear_paginas(player, x2, y2, pd);
}

void mover_pantalla(int x, int y, int x2, int y2, u8 tipo){
	x+=1;
	x2+=1;
	y++;
	y2++;
	print_string("X", x, y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (y2<=0){
		y2 += ALTO_MAPA;
	}
	if (y2>=ALTO_MAPA+1){
		y2 -= ALTO_MAPA;
	}
	if (tipo == 0)
		print_string("G", x2, y2, getFormat(C_FG_BLACK,0,C_BG_GREEN,0));
	if (tipo == 1)
		print_string("M", x2, y2, getFormat(C_FG_BLACK,0,C_BG_GREEN,0));
	if (tipo == 2)
		print_string("C", x2, y2, getFormat(C_FG_BLACK,0,C_BG_GREEN,0));
	if(tipo == ZOMBIE_MUERTO)
		print_string("X", x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
}

void show_debugger(registers* regs) {
    print_debugger(currentPosA, zombieClassA, regs);
}

void movimiento(page_directory* pd ,int delta_x,int delta_y,u8 tipo){

	unsigned int x;
	unsigned int y;
	get_position(&x,&y,recuperar_fisica(0x8000000,pd));

    unsigned int player = (recuperar_fisica(0x8001000, pd) < recuperar_fisica(0x8000000, pd));
    int x2 = (int) x;
    int y2 = (int) y;
    if(!player){
    	x2 += delta_x;
    	y2 += delta_y;
    }else{
    	y2 -= delta_y;
    	x2 -= delta_x;
    }

    if(x2<=0){
    	mover_pantalla(x, y, x2, y2, ZOMBIE_MUERTO);
    	sumarPunto(1);
    }else if(x2>=ANCHO_MAPA-1){
    	mover_pantalla(x, y, x2, y2, ZOMBIE_MUERTO);
		sumarPunto(0);
    }else{
		mover_soldado(x2, y2, player, pd);    
		mover_pantalla(x, y, x2, y2, tipo);
	}
}


void game_move_current_zombi(direccion dir) {
	breakpoint();
	contadorDeInactividad = 0;
	if(dir==ADE){
		memcpy((void*)0x8000000,(void*)0x8001000,0x1000);
		movimiento((page_directory*)rcr3(),1,0,claseActual);
	}else if(dir==DER){
		memcpy((void*)0x8000000,(void*)0x8004000,0x1000);
		movimiento((page_directory*)rcr3(),0,1,claseActual);
	}else if(dir==ATR){
		memcpy((void*)0x8000000,(void*)0x8006000,0x1000);
		movimiento((page_directory*)rcr3(),-1,0,claseActual);
	}else if (dir == IZQ) {
		memcpy((void*)0x8000000,(void*)0x8005000,0x1000);
		movimiento((page_directory*)rcr3(),0,-1,claseActual);
	} else {
		printf(1, 1, "%s", "DIRECCION INVALIDA");		
	}
}

void revisarTerminacion(){
	if(!termino_el_juego){
		if((cantZombiesA == 0 && zombiesActivosA==0) && (cantZombiesB == 0 && zombiesActivosB==0)){
			terminar_juego();
		}else{
			revisar_terminar_por_inactividad();
		}
	}
}

void mostrarCartelDeFin(char* text){
	print_string(text,32,22,getFormat(C_FG_WHITE,0,C_BG_RED,0));
}

void terminar_juego(){
	if(puntajeA==puntajeB){
		mostrarCartelDeFin("    EMPATE    ");
	}else if(puntajeA>puntajeB){
		mostrarCartelDeFin("GANO JUGADOR A");
	}else{
		mostrarCartelDeFin("GANO JUGADOR B");
	}
	termino_el_juego = 1;
}

u8 puedeLanzarZombies(u8 player){
	if(player){
		return (zombiesActivosB != 8 && cantZombiesB > 0);
	}else{
		return (zombiesActivosA != 8 && cantZombiesA > 0);
	}
}

void revisar_terminar_por_inactividad(){
	if(!puedeLanzarZombies(0) && !puedeLanzarZombies(1)){
		if(contadorDeInactividad>=LIMITE_DE_TIEMPO_INACTIVIDAD){
			terminar_juego();
		}
		contadorDeInactividad++;
	}else{
		contadorDeInactividad = 0;
	}
}