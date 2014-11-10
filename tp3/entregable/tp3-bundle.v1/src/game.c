/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "sched.h"


void game_init() {
	currentPosA = 20;
	currentPosB = 20;
	zombieClassA = MAGO;
	zombieClassB = MAGO;
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
