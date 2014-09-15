
global bandas_asm

section .data

color1 db 0,0,0,255
color2 db 64,64,64,255,
color3 db 128,128,128,255
color4 db 192,192,192,255
color5 db 255,255,255,255

all_64w dw 64,64,64,64,64,64,64,64


color1_bound dw 96,96,96,96,96,96,96,96
color2_bound dw 288,288,288,288,288,288,288,288
color3_bound dw 480,480,480,480,480,480,480,480
color4_bound dw 672,672,672,672,672,672,672,672

rgb_only_mask dd 0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF

all_1_mask    dd 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
all_0_mask    dd 0x00000000,0x00000000,0x00000000,0x00000000

magic_shuffle db 8,8,8,8,10,10,10,10,0,0,0,0,2,2,2,2


section .text
;void bandas_asm    (
;     unsigned char *src, rdi
;     unsigned char *dst, rsi
;     int m,              rdx
;     int n,              rcx
;     int src_row_size,   r9
;     int dst_row_size)   r8
bandas_asm:
	push rbp
	mov rbp,rsp
	push r13
	push r14
	push r15

    xor r15,r15 ; x
    xor r14,r14 ; y
    pxor xmm15,xmm15
.loop_y:
    cmp r14d,ecx
    je .endloop_y
.loop_x:
    cmp r15d,edx
    je .endloop_x

    ; armo el offset
    xor rax,rax
    mov r12d,r14d ; r12 = y
    mov r13d,r15d ; r13 = x
    imul r12d,r9d ; y * row_size
    imul r13d,4   ; x * 4 (esto puede ser un shift left)
    mov eax,r12d  ; 
    add eax,r13d  ; rax = y*row_size + x*4 

    movdqu xmm1,[rdi+rax] ; agarro 16 bytes del source
    movdqu xmm2,xmm1

    ; Pongo ceros en alpha así no jode en la suma 
    ; (también podría aumentar en 255 los valores con los que comparo)
    movdqu xmm15,[rgb_only_mask]
    pand xmm1,xmm15
    pand xmm2,xmm15

    pxor xmm7,xmm7
    punpcklbw xmm1,xmm7 ; baja 0
    punpckhbw xmm2,xmm7 ; alta 8

    pxor xmm7,xmm7
    pxor xmm8,xmm8

    phaddw xmm1,xmm7
    phaddw xmm7,xmm1 
    
    movdqu xmm1,xmm7

    phaddw xmm2,xmm8
    phaddw xmm2,xmm8

    paddw xmm1,xmm2

    movdqu xmm15,[color1_bound]
    movdqu xmm0,xmm1 

    pcmpgtw xmm1,xmm15 ; pone ceros en dst donde no se cumpla la condición (greater than)

    movdqu xmm2,xmm0
    movdqu xmm15,[color2_bound]
    pcmpgtw xmm2,xmm15 

    movdqu xmm3,xmm0
    movdqu xmm15,[color3_bound]
    pcmpgtw xmm3,xmm15 

    movdqu xmm4,xmm0
    movdqu xmm15,[color4_bound]
    pcmpgtw xmm4,xmm15 

    paddw xmm1,xmm2
    paddw xmm1,xmm3
    paddw xmm1,xmm4

    pabsw xmm1,xmm1

    movdqu xmm0,[all_64w]
    pmullw xmm1,xmm0

    ; reordeno los bytes a como estaba original
    movdqu xmm15,[magic_shuffle] 
    pshufb xmm1,xmm15

    movdqu [rsi+rax],xmm1

    add r15,4 ; avanzo 4 sobre x
    jmp .loop_x
.endloop_x:
    inc r14
    xor r15,r15 ; reinicio x
    jmp .loop_y
.endloop_y:

	pop r15
	pop r14
	pop r13
	pop rbp
    ret
 
