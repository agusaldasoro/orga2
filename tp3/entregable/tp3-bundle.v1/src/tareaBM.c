/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h" 

void task() {
    /* Tarea */
    int i;
    //breakpoint();
    for(i=0;i<10;i++) {
    syscall_mover(ADE); //breakpoint();
    }
    for(i=0;i<10;i++) {
    syscall_mover(DER); //breakpoint();
    }
    for(i=0;i<10;i++) {
    syscall_mover(ADE); //breakpoint();
    }
    for(i=0;i<10;i++) {
    syscall_mover(IZQ); //breakpoint();
    }
    for(i=0;i<10;i++) {
    syscall_mover(ADE); //breakpoint();
    }
    
//   while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}