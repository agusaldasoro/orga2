/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

unsigned short sched_proximo_indice() {
  return 0;
}

void start_zombie(u8 player, u8 class, u8 y) {
	page_directory* pd = mmu_inicializar_dir_zombie(player, class, y);
	init_tss(get_free_tss(player), (u32) pd, ZOMBIE_VIRTUAL, ZOMBIE_VIRTUAL + PAGE_SIZE, 0x48, 0x58, 0x202);
}