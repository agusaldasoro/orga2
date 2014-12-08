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


void game_init() {
	puntajeA = 0;
	puntajeB = 0;
	currentPosA = 20;
	currentPosB = 20;
	zombieClassA = MAGO;
	zombieClassB = MAGO;
}

void sumarPuntoA(){
	puntajeA++;
	if(puntajeA==15){/*TODO terminoElJuego*/}
}

void sumarPuntoB(){
	puntajeB++;
	if(puntajeB==15){/*TODO terminoElJuego*/}
}

void game_change_class(u8 player, u8 d) {
	if (player) {
		zombieClassB = (zombieClassB+d) % 3;
	} else {
		zombieClassA = (zombieClassA+d) % 3;
	}
}

void game_move_zombie(u8 player, u8 d) {
	//TODO
	if (player) {
		if (currentPosB <= 1 || currentPosB >= 40) return;
		currentPosB = currentPosB + d;
	} else {
		if (currentPosA <= 1 || currentPosA >= 40) return;
		currentPosA = currentPosA + d;
	}


}


void game_jugador_mover(unsigned int jugador, unsigned int value) {

}

void game_lanzar_zombi(u8 player) {
	breakpoint();
	if (player) {
		start_zombie(player, zombieClassB, currentPosB);
	} else {
		start_zombie(player, zombieClassA, currentPosA);
	}

}

u8 mover_soldado(int x2, int y2,unsigned int player, page_directory* pd) {
	char* text;

    if(x2<=0){
    	text = "me declaro spectrum",0;
    	print_string(text, 22, 22, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));
    	sumarPuntoB();
    	return 0;
    }else if(x2>=ANCHO_MAPA-1){
    	text = "me declaro spectrum15",0;
    	print_string(text, 22, 22, getFormat(C_FG_WHITE, 0, C_BG_BLACK  , 0));
    	sumarPuntoA();
    	return 0;
    }else{
    	setear_paginas (player, x2, y2, pd);
    	return 1;
    }
}

void mover_pantalla(int x,int y,int x2,int y2,u8 tipo){
	x+=1;
	x2+=1;
	print_string("X",x, y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if(y2<0){
		y2 += ALTO_MAPA;
	}
	if(y2>=ALTO_MAPA){
		y2 -= ALTO_MAPA;
	}
	if (tipo==0)
		print_string("G",x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo==1)
		print_string("M",x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo==2)
		print_string("C",x2, y2, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
}

/* pd:eax,delta_x:edi,delta_y:esi, tipo: dx */
void movimiento(page_directory* pd ,int delta_x,int delta_y,u8 tipo){

	unsigned int x;
	unsigned int y;
	get_position(&x,&y,recuperar_fisica(0x8000000,pd));

    unsigned int player = (recuperar_fisica(0x8001000,pd)<recuperar_fisica(0x8000000,pd));
    int x2 = (int) x;
    int y2 = (int) y;
    if(!player){
    	x2 += delta_x;
    	y2 += delta_y;
    }else{
    	x2 -= delta_x;
    	y2 -= delta_y;
    }


	u8 se_movio = mover_soldado(x2,y2,player,pd);
	if(se_movio)mover_pantalla(x,y,x2,y2,tipo);
}


void game_move_current_zombi(direccion dir) {
	if(dir==ADE){
		memcpy((void*)0x8000000,(void*)0x8001000,0x1000);
		movimiento((page_directory*)rcr3(),1,0,0);
	}else if(dir==DER){
		memcpy((void*)0x8000000,(void*)0x8004000,0x1000);
		movimiento((page_directory*)rcr3(),0,1,0);
	}else if(dir==ATR){
		memcpy((void*)0x8000000,(void*)0x8006000,0x1000);
		movimiento((page_directory*)rcr3(),-1,0,0);
	}else{
		memcpy((void*)0x8000000,(void*)0x8005000,0x1000);
		movimiento((page_directory*)rcr3(),0,-1,0);
	}
}