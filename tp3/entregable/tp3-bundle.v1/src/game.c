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
}

void mostrar_cursores(u8 player,s8 d){
	if(!player){
		print_string(" ",0,currentPosA, getFormat(C_FG_WHITE, 0, C_BG_RED, 0));
		print_string("G",0,currentPosA+d, getFormat(C_FG_WHITE, 0, C_BG_RED, 0));
	}else{
		print_string(" ",79,currentPosB, getFormat(C_FG_WHITE, 0, C_BG_BLUE, 0));
		print_string("G",79,currentPosB+d, getFormat(C_FG_WHITE, 0, C_BG_BLUE, 0));
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

void sumarPuntoA(){
	puntajeA++;
	reimprimirPuntaje(0);
	if(puntajeA==15){/*TODO terminoElJuego*/}
}

void sumarPuntoB(){
	puntajeB++;
	reimprimirPuntaje(1);
	if(puntajeB==15){/*TODO terminoElJuego*/}
}

void game_change_class(u8 player, s8 d) {
	if (player) {
		zombieClassB = (zombieClassB+d) % 3;
	} else {
		zombieClassA = (zombieClassA+d) % 3;
	}
}

void game_move_zombie(u8 player, s8 d) {
	//TODO
	if (player) {
		if (currentPosB <= 0 || currentPosB >= ALTO_MAPA-1) return;
		mostrar_cursores(player,d);
		currentPosB = currentPosB + d;
	} else {
		if (currentPosA <= 0 || currentPosA >= ALTO_MAPA-1) return;
		mostrar_cursores(player,d);
		currentPosA = currentPosA + d;
	}
}


void game_jugador_mover(unsigned int jugador, unsigned int value) {

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

u8 mover_soldado(int x2, int y2,unsigned int player, page_directory* pd) {
	// char* text;
	breakpoint();
    if(x2<=0){
    	// text = "me declaro spectrum";
    	// print_string(text, 22, 22, getFormat(C_FG_WHITE, 0, C_BG_BLACK, 0));
		// breakpoint();
    	sumarPuntoB();
    	return 0;
    }else if(x2>=ANCHO_MAPA-1){
    	// text = "me declaro spectrum15";
    	// print_string(text, 22, 22, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));
    	// breakpoint();
		sumarPuntoA();
    	return 0;
    }else{
    	breakpoint();
		setear_paginas(player, x2, y2, pd);
    	return 1;
    }
}

void mover_pantalla(int x, int y, int x2, int y2, u8 tipo){
	x+=1;
	x2+=1;
	print_string("X", x, y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (y2<0){
		y2 += ALTO_MAPA;
	}
	if (y2>=ALTO_MAPA){
		y2 -= ALTO_MAPA;
	}
	if (tipo == 0)
		print_string("G", x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo == 1)
		print_string("M", x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo == 2)
		print_string("C", x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
}

/* pd:eax,delta_x:edi,delta_y:esi, tipo: dx */
void movimiento(page_directory* pd ,int delta_x,int delta_y,u8 tipo){

	unsigned int x;
	unsigned int y;
	get_position(&x,&y,recuperar_fisica(0x8000000,pd));

	// TODO : Por dios, explicar la magia atrás de esta línea.
    unsigned int player = (recuperar_fisica(0x8001000, pd) < recuperar_fisica(0x8000000, pd));
    int x2 = (int) x;
    int y2 = (int) y;
    if(!player){
    	x2 += delta_x;
    	y2 += delta_y;
    }else{
    	x2 -= delta_x;
    	y2 -= delta_y;
    	x2 -= delta_x;
    }


	u8 se_movio = mover_soldado(x2, y2, player, pd);
	if (se_movio) mover_pantalla(x, y, x2, y2, tipo);
}


void game_move_current_zombi(direccion dir) {
	breakpoint();
	if(dir==ADE){
		memcpy((void*)0x8000000,(void*)0x8001000,0x1000);
		movimiento((page_directory*)rcr3(),1,0,0);
	}else if(dir==DER){
		memcpy((void*)0x8000000,(void*)0x8004000,0x1000);
		movimiento((page_directory*)rcr3(),0,1,0);
	}else if(dir==ATR){
		memcpy((void*)0x8000000,(void*)0x8006000,0x1000);
		movimiento((page_directory*)rcr3(),-1,0,0);
	}else if (dir == IZQ) {
		memcpy((void*)0x8000000,(void*)0x8005000,0x1000);
		movimiento((page_directory*)rcr3(),0,-1,0);
	} else {
		printf(1, 1, "%s", "GUACHOS FORROS");		
	}
}