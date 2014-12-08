/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "tss.h"

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(unsigned int jugador, unsigned int value);

void game_lanzar_zombi(u8 jugador);

void game_move_current_zombi(direccion dir);
void mostrar_cursores(u8 player,s8 d);


u32 currentPosA;
u32 currentPosB;
u8 zombieClassA;
u8 zombieClassB;


void game_init();
void game_change_class(u8 player, s8 d);
void game_move_zombie(u8 jugador, s8 d);

void reimprimirCantidadZombies(u8 player);
char* get_zombie_type(u32 type);
void terminar_juego();
void revisarTerminacion();
void revisar_terminar_por_inactividad();
unsigned int contadorDeInactividad;

#endif  /* !__GAME_H__ */
