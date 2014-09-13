global sierpinski_asm

section .data

offset: dd 3.0, 2.0, 1.0, 0.0
dq255:  dd 255.0,255.0,255.0,255.0

section .text

;void sierpinski_asm (unsigned char *src,
;                     unsigned char *dst,
;                     int cols, int rows,
;                     int src_row_size,
;                     int dst_row_size)


; void sierpinski_c    (
;     unsigned char *src, rdi
;     unsigned char *dst, rsi
;     int cols,           rdx
;     int filas,          rcx
;     int src_row_size,   r9
;     int dst_row_size)   r8
; {
sierpinski_asm:
    push rbp
    mov rbp, rsp
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15 

    xor r15,r15 ; x
    xor r14,r14 ; y
    pxor xmm15,xmm15
.loop_x:
    cmp r14d,ecx
    je .endloop_x
.loop_y:
    cmp r15d,edx
    je .endloop_y

    xor rax,rax 
    mov rax,r15           ; rax = x
    cvtsi2ss xmm1,rax     ; rax = (float) rax y lo muevo a xmm1
    shufps xmm1, xmm1, 0h ; replico rax en todo xmm1
    movups xmm2,[offset]  ; 
    addps xmm1,xmm2       ; le sumo el offset de x en los pixels (3,2,1,0)
    mov rax,r14           ; rax = y
    cvtsi2ss xmm2,rax     ; rax = (float) rax y lo muevo a xmm2
    shufps xmm2, xmm2, 0h ; replico rax en todo xmm2


    ; esto puede optimizarse haciendo 255/filas y 255/cols una única vez
    ; en lugar de hacer en cada iteración
    movups xmm0,[dq255]
    mulps xmm1,xmm0 ; x*255
    mulps xmm2,xmm0 ; y*255

    ; int i = (int) ( floor((x*255.0)/filas) );
    ; int j = (int) ( floor((y*255.0)/cols) );

    cvtsi2ss xmm3,rdx     
    shufps xmm3, xmm3, 0h ; filas

    cvtsi2ss xmm4,rcx
    shufps xmm4,xmm4, 0h ; cols

    divps xmm1,xmm3 
    divps xmm2,xmm4

    cvttps2dq xmm1,xmm1 ; convierte a int truncando, o sea ((int) floor(n))
    cvttps2dq xmm2,xmm2

    pxor xmm1,xmm2 ; hago el xor entre los 4 Is y los 4 Js empaquetados

    movups xmm0,[dq255]
    cvtdq2ps xmm1,xmm1 ; los convierto a float otra vez
    divps xmm1,xmm0    ; y los divido por 255.0 

    ; ahora en xmm1 tengo los 4 coefiecientes 
    ; que corresponden a los próximos cuatro PIXELS
    ; que vamos a procesar!

    movups xmm15,xmm1  ; me hago una copia de los coeficientes porque pintó.
    ; float s = (i xor j) / 255.0;

    xor rax,rax
    mov r12d,r14d ; r12 = y
    mov r13d,r15d ; r13 = x
    imul r12d,r9d ; y * row_size
    imul r13d,4   ; x * 4 (esto puede ser un shift left)
    mov eax,r12d  ; 
    add eax,r13d  ; rax = y*row_size + x*4 

    
    movdqu xmm0,[rdi+rax] ; agarró 16 bytes del source!
    movdqu xmm1,xmm0      ; y me hago una copia

    pxor xmm2,xmm2

    punpcklbw xmm0,xmm2 ; parte baja 
    movdqu xmm6,xmm0    ; copio la parte baja
    punpcklwd xmm0,xmm2 ; baja de la baja 0 
    punpckhbw xmm6,xmm2 ; alta de la baja 1

    punpckhbw xmm1,xmm2 ; alta           
    movdqu xmm7,xmm1    ; copio la parte alta
    punpckhwd xmm1,xmm2 ; alta de alta    3
    punpcklwd xmm7,xmm2 ; baja de la alta 2

    cvtdq2ps xmm0,xmm0
    cvtdq2ps xmm6,xmm6
    cvtdq2ps xmm1,xmm1
    cvtdq2ps xmm7,xmm7

    movdqu xmm14,xmm15
    shufps xmm15,xmm15, 0x00 ; 00 00 00 00
    ; mulps xmm0,xmm15
    mulps xmm1,xmm15

    movdqu xmm15,xmm14
    shufps xmm15,xmm15, 0x55 ; 01 01 01 01 
    mulps xmm7,xmm15       
    ; mulps xmm6,xmm15       
    
    movdqu xmm15,xmm14
    shufps xmm15,xmm15, 0xAA ; 10 10 10 10
    mulps xmm6,xmm15
    ; mulps xmm7,xmm15
    
    movdqu xmm15,xmm14
    shufps xmm15,xmm15, 0xFF ; 11 11 11 11
    ; mulps xmm1,xmm15
    mulps xmm0,xmm15

    ; Convierto los pixels (floats) a ints empaquetados!
    cvtps2dq xmm1,xmm1
    cvtps2dq xmm0,xmm0
    cvtps2dq xmm6,xmm6
    cvtps2dq xmm7,xmm7

    ;De ints a shorts
    packusdw xmm6,xmm0
    packusdw xmm7,xmm1

    ; Y de shorts a chars :D
    packuswb xmm6,xmm7

    ; por qué necesitamos unsigned saturation? Marco señal.

    movdqa [rsi+rax],xmm6


    add r15,4 ; avanzo 4 sobre x
    jmp .loop_y
.endloop_y:
    inc r14
    xor r15,r15 ; reinicio x
    jmp .loop_x
.endloop_x:

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop rbp
    ret