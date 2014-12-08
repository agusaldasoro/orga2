; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start
extern GDT_DESC
extern IDT_DESC

extern idt_inicializar
extern mmu_inicializar_dir_kernel
extern mmu_inicializar

extern clear_screen
extern print_map

extern habilitar_pic
extern resetear_pic

; extern gdt_init
extern tss_inicializar
extern GDT_TSS_IDLE

;extern entrarEnIdl
;; Saltear seccion de datos

%define GDT_TSS_SELEC (GDT_TSS_IDLE * 8)

jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    

    ; Habilitar A20
	call habilitar_A20
    ;xchg bx, bx
    ; Cargar la GDT

	lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
	mov eax,cr0
	or eax,1
	mov cr0,eax

	jmp 0x50:modo_protegido

BITS 32
modo_protegido:

    ; Saltar a modo protegido

    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax, 0x40
     
    mov es, ax
    mov ds, ax
    mov ss, ax    
    mov gs, ax

    mov ax, 0x60 
    mov fs, ax



    ; Establecer la base de la pila
    mov ebp, 0x27000
    mov esp, 0x27000

    ; imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len,0x07,0,0

    ; Imprimir mensaje de bienvenida


    ; Inicializar pantalla
    call clear_screen

    call print_map

    ; Inicializar el manejador de memoria
    ; Inicializar el directorio de paginas
    ; Cargar directorio de paginas

    call mmu_inicializar
        
    ; Habilitar paginacion
    xchg bx, bx

    mov eax,0x27000
    mov cr3,eax

    mov eax,cr0
    or eax,0x80000000
    mov cr0,eax


    ;mov eax,0x100000
    ;mov cr3,eax

    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    call tss_inicializar

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar
    ; Cargar IDT
    lidt [IDT_DESC]

    xor eax, eax
    mov ax,0x68
    ltr ax
    xchg bx, bx


    ; ; test para que salte la divide by 0 exception (0)
    ; mov edx,0
    ; mov ecx,0
    ; mov eax,3
    ; div ecx
 
    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic
    ; Cargar tarea inicial

    extern game_init
    call game_init

    extern start_zombie

    ;xchg bx, bx
    ;xor ax,ax
    ;mov ax,0
    ;push ax
    ;push ax
    ;mov ax,0
    ;push ax
    ;xchg bx, bx
    ;call start_zombie

    extern game_lanzar_zombi

    ;mov eax,0
    ;push eax
    ;call game_lanzar_zombi
    ;pop eax

    ;mov eax,1
    ;push eax
    ;call game_lanzar_zombi
    ;pop eax

    ;xchg bx, bx
    ;jmp 0x80:modo_protegido

    xchg bx, bx
    ; Habilitar interrupciones
    sti
    ; Saltar a la primera tarea: Idle
    ;call entrarEnIdl
    jmp 0x80:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
