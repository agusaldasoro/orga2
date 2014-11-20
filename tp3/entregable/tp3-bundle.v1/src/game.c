/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "sched.h"
#include "screen.h"
#include "mmu.h"

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

void game_lanzar_zombi(unsigned int player) {
	if (player) {
		start_zombie(player, zombieClassB, currentPosB);
	} else {
		start_zombie(player, zombieClassA, currentPosA);
	}

}

void game_move_current_zombi(direccion dir) {
	//if(dir==ADE){
	//	movimiento(pd,1,0);
	//}else if(dir==DER){
	//	movimiento(pd,0,1);
	//}else if(dir==ATR){
	//	movimiento(pd,-1,0);
//	}else{
//		movimiento(pd,0,-1);
//	}
}



void mover_soldado(int delta_x, int delta_y, page_directory* pd) {
	desplazar_fisica(0x8000000, pd, delta_x, delta_y);
    if(recuperar_fisica(0x8000000,pd)%(50*0x1000)==0){
    	sumarPuntoB();
    }else if(recuperar_fisica(0x8000000,pd)%(49 * 0x1000)){
    	sumarPuntoA();
    }else{
    	int i = 1;
    	while(i<9){
        	desplazar_fisica(0x8000000+i*0x1000, pd, delta_x, delta_y);
        	i++;
    	}
    }
}

void mover_pantalla(int x,int y,int delta_x,int delta_y,u8 tipo){
	print_string("X",x, y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo==0)
		print_string("G",x+delta_x, y+delta_y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo==1)
		print_string("M",x+delta_x, y+delta_y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
	if (tipo==2)
		print_string("C",x+delta_x, y+delta_y, getFormat(C_FG_LIGHT_GREY,0,C_BG_GREEN,0));
}

/* pd:eax,delta_x:edi,delta_y:esi, tipo: dx */
void movimiento(page_directory* pd ,int delta_x,int delta_y,u8 tipo){
	mover_soldado(delta_x,delta_y,pd);
	unsigned int x;
	unsigned int y;
	get_position(&x,&y,recuperar_fisica(0x8000000,pd));
	mover_pantalla(x,y,delta_x,delta_y,tipo);
}

void movimiento_direccional(direccion d){
	if(d==ADE){
		movimiento((page_directory*)rcr3(),1,0,0);
	}else if(d==DER){
		movimiento((page_directory*)rcr3(),0,1,0);
	}else if(d==ATR){
		movimiento((page_directory*)rcr3(),-1,0,0);
	}else{
		movimiento((page_directory*)rcr3(),0,-1,0);
	}
}