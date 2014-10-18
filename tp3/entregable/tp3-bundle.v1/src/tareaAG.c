/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h" 

void task() {
    /* Tarea */
    int i;
    for(i=0;i<100;i++) {
      syscall_mover(ADE); 
      syscall_mover(DER); 
      syscall_mover(ADE); 
      syscall_mover(ADE); 
      syscall_mover(IZQ);
      syscall_mover(ADE); 
    }
  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}