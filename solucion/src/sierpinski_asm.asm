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
    ; mov qword [rbp-20], 0

    xor r15,r15 ; x
    xor r14,r14 ; y
    pxor xmm15,xmm15
.loop_x:
    cmp r14,rdx
    je .endloop_x
.loop_y:
    cmp r15,rcx
    je .endloop_y


    xor rax,rax 
    mov rax,r15
    cvtsi2ss xmm1,rax
    shufps xmm1, xmm1, 0h
    movups xmm2,[offset]
    addps xmm1,xmm2
    mov rax,r14
    cvtsi2ss xmm2,rax
    shufps xmm2, xmm2, 0h

    movups xmm0,[dq255]

    mulps xmm1,xmm0 ; x
    mulps xmm2,xmm0 ; y

    ; int i = (int) ( floor((x*255.0)/filas) );
    ; int j = (int) ( floor((y*255.0)/cols) );

    cvtsi2ss xmm3,rdx
    shufps xmm3, xmm3, 0h ; filas

    cvtsi2ss xmm4,rcx
    shufps xmm4,xmm4, 0h ; cols

    divps xmm1,xmm3
    divps xmm2,xmm4
    ; float s = (i xor j) / 255.0;

    cvttps2dq xmm1,xmm1
    cvttps2dq xmm2,xmm2

    pxor xmm1,xmm2

    movups xmm0,[dq255]

    cvtdq2ps xmm1,xmm1

    divps xmm1,xmm0
    movdqu xmm15,xmm1

    ; hasta acà tengo 4 coeficientes

    ; en xmm1 tengo los 4 coeficientes

    xor rax,rax
    mov r12d,r14d
    mov r13d,r15d
    imul r12d,r9d ; y * row_size
    imul r13d,4   ; x * 4 (esto puede ser un shift left)
    mov eax,r12d  ; 
    add eax,r13d  ; rax = y*row_size + x*4 

    mov r11,[rdi+rax]
    
    pxor xmm0,xmm0


    movdqu xmm0,[rdi+rax]
    movdqu xmm1,xmm0

    ; pxor xmm1,xmm1
    pxor xmm2,xmm2

    punpcklbw xmm0,xmm2 ; parte baja 
    movdqu xmm6,xmm0    ; 
    punpcklwd xmm0,xmm2 ; baja de la baja 0 
    punpckhbw xmm6,xmm2 ; alta de la baja 1

    punpckhbw xmm1,xmm2 ; alta           
    movdqu xmm7,xmm1    ; 
    punpckhwd xmm1,xmm2 ; alta de alta    3
    punpcklwd xmm7,xmm2 ; baja de la alta 2

    CVTDQ2PS xmm0,xmm0
    CVTDQ2PS xmm6,xmm6
    CVTDQ2PS xmm1,xmm1
    CVTDQ2PS xmm7,xmm7

    movups xmm14,xmm15
    shufps xmm15,xmm15, 0x00 ; 00 00 00 00
    mulps xmm0,xmm15

    movups xmm15,xmm14
    shufps xmm15,xmm15, 0x55 ; 01 01 01 01
    mulps xmm7,xmm15
    
    movups xmm15,xmm14
    shufps xmm15,xmm15, 0xAA ; 10 10 10 10
    mulps xmm6,xmm15
    
    movups xmm15,xmm14
    shufps xmm15,xmm15, 0xFF ; 11 11 11 11
    mulps xmm1,xmm15

;floats 
    cvtps2dq xmm1,xmm1
    cvtps2dq xmm0,xmm0
    cvtps2dq xmm6,xmm6
    cvtps2dq xmm7,xmm7
;ints

    packusdw xmm6,xmm0
    packusdw xmm7,xmm1
;shorts
    PACKSSWB xmm7, xmm6

    movdqu [rsi+rax],xmm7    

    ; mov [rsi+rax],r11

    add r15,4 ; avanzo 4
    ; inc r15
    jmp .loop_y
.endloop_y:
    ; add r14,4
    inc r14
    xor r15,r15
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