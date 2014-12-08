; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

registers_snapshot dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

keyboard_str:   db "Teclado: %h",0


;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice

extern print_exception
extern proximo_indice
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1 

_isr%1:
    ; xchg bx, bx
    mov [registers_snapshot],eax
    mov [registers_snapshot+4],ebx
    mov [registers_snapshot+8],ecx
    mov [registers_snapshot+12],edx
    mov [registers_snapshot+16],edi
    mov [registers_snapshot+20],esi
    mov [registers_snapshot+24],ebp
    mov [registers_snapshot+28],esp
    mov [registers_snapshot+32],cs
    mov [registers_snapshot+36],ds
    mov [registers_snapshot+40],es
    mov [registers_snapshot+44],fs
    mov [registers_snapshot+48],gs
    mov [registers_snapshot+52],ss
    mov eax,0
    lea ebx,[registers_snapshot+56]
.stack:
    cmp eax,10
    je .endstack
    mov [ebx+eax*4],ebp
    inc eax
    jmp .stack
.endstack:
    mov eax, %1
    push registers_snapshot
    push eax
    call print_exception
    jmp $

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0 ; _isr0
ISR 1 ; _isr0
ISR 2 ; _isr0
ISR 3 ; _isr0
ISR 4 ; _isr0
ISR 5 ; _isr0
ISR 6 ; _isr0
ISR 7 ; _isr0
ISR 8 ; _isr0
ISR 9 ; _isr0
ISR 10 ; _isr0
ISR 11 ; _isr0
ISR 12 ; _isr0
ISR 13 ; _isr0
ISR 14 ; _isr0
ISR 15 ; _isr0
ISR 16 ; _isr0
ISR 17 ; _isr0
ISR 18 ; _isr0
ISR 19 ; _isr0

; global _isr14
; _isr14:
;     jmp 0x80:0


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
     ; xchg bx, bx
    pushad
    call proximo_reloj
    call proximo_indice

    cmp ax,0
    je .nojump

    mov [sched_tarea_selector], ax
    call fin_intr_pic1
    popad
    ; xchg bx, bx
    jmp far [sched_tarea_offset]
    jmp .end

.nojump:
    call fin_intr_pic1
    popad

.end:
    ; switchear tareas.
    ; xchg bx, bx
    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
extern printf
extern print_int
extern handle_keyboard_interrumption
_isr33:
    pushad
    xor eax,eax
    in al, 0x60

    mov dword [esp], eax
    call fin_intr_pic1
    call handle_keyboard_interrumption

    popad
    iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


global _isr102
extern game_move_current_zombi
_isr102:
    pushad
    push eax
    
    call game_move_current_zombi
    pop eax
    popad
    jmp 0x80:0
    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
        
        
