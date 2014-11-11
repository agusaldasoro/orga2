/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "sched.h"

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



}
void mover_soldado(int x, int y, page_directory* pd) {
    int i = 0;
    if(recuperar_fisica(0x8000000,pd)%(50*0x1000)==0){
    	sumarPuntoB();
    }else if(recuperar_fisica(0x8000000,pd)%(49 * 0x1000)){
    	sumarPuntoA();
    }else{
    	while(i<9){
        	desplazar_fisica(0x8000000+i*0x1000, pd, x, y);
        	i++;
    	}
    }
}
