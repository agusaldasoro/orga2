/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "mmu.h"
#include "tss.h"

unsigned short sched_proximo_indice();
void start_zombie(u8 player, u8 class, u32 y);


#endif	/* !__SCHED_H__ */
