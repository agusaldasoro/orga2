
global mblur_asm

section .data

cerocomados dd 0.2,0.2,0.2,0.2
rgb_only    dd 0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF


section .text
;void mblur_asm    (
;   unsigned char *src, RDI
;   unsigned char *dst, RSI
;   int cols,           EDX 
;   int filas,          ECX
;   int src_row_size,   R8D
;   int dst_row_size)   R9D

mblur_asm:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    sub rsp,8

    xor r10,r10 ; i
    xor r11,r11 ; j
    xor r15,r15
    xor r14,r14
    pxor xmm13,xmm13

    mov r15d,edx ; r15 = cols
    mov r14d,ecx ; r14 = filas
    sub r15,2    ; r15 = cols -2 (porque las filas y columnas de dos pixels del borde van de negro)
    sub r14,2    ; r14 = filas - 2  
    

.loop_y:
    cmp r10d,ecx    ; i < filas 
    jge .endloop_y
    
    mov r11,0       ; j = 0

.loop_x:
    cmp r11d,edx ; j < cols 
    jge .endloop_x

    mov rax,r10  ; rax = i
    imul eax,r8d ; rax = i * row_size 
    lea rax,[rax+r11*4] ; rax = i * row_size + j * 4

    cmp r11,2   
    jl .cero    ; j < 2
    cmp r10,2 
    jl .cero    ; i < 2
    cmp r10,r14
    jge .cero   ; i >= cols -2
    cmp r11,r15
    jge .cero   ; j >= filas -2

    jmp .nocero

.cero:
    movq [rsi+rax],xmm13        
    add r11,2
    jmp .loop_x

.nocero:
    
    lea rbx,[rdi+rax]
    movdqu xmm1,[rbx]        ; (i,j)
    movdqu xmm2,[rbx+r8*1+4] ; (i+1,j+1)
    movdqu xmm3,[rbx+r8*2+8] ; (i+2,j+2)
    mov r12,rbx
    sub r12,r8
    movdqu xmm4,[r12-4]      ; (i-1,j-1)
    sub r12,r8
    movdqu xmm5,[r12-8]      ; (i-2,j-1)

    movdqu xmm14,[rgb_only]

    pxor xmm0,xmm0
    movdqu xmm6,xmm1      ; xmm6 = (i,j), (i,j+1), (i,j+2), (i,j+3)
    punpcklbw xmm1,xmm0   ; xmm1 = (i,j), (i,j+1)
    punpckhbw xmm6,xmm0   ; xmm6 = (i,j+2), (i,j+3)

    movdqu xmm7,xmm2      ; xmm7 = (i+1,j+1), (i+1,j+2), (i+1,j+3), (i+1,j+4)
    punpcklbw xmm2,xmm0   ; xmm2 = (i+1,j+1), (i+1,j+2)
    punpckhbw xmm7,xmm0   ; xmm7 = (i+1,j+3), (i+1,j+4)

    movdqu xmm8,xmm3      ; xmm8 = (i+2,j+2), (i+2,j+3), (i+2,j+4), (i+2,j+5)
    punpcklbw xmm3,xmm0   ; xmm3 = (i+2,j+2), (i+2,j+3)
    punpckhbw xmm8,xmm0   ; xmm8 = (i+2,j+4), (i+2,j+5)

    movdqu xmm9,xmm4      ; xmm9 = (i-1,j-1), (i-1,j), (i-1,j+1), (i-1,j+2)
    punpcklbw xmm4,xmm0   ; xmm4 = (i-1,j-1), (i-1,j)
    punpckhbw xmm9,xmm0   ; xmm9 = (i-1,j+1), (i-1,j+2)

    movdqu xmm10,xmm5     ; xmm10 = (i-2,j-1), (i-2,j), (i-2,j+1), (i-2,j+2)
    punpcklbw xmm5,xmm0   ; xmm5 = (i-2,j-1), (i-2,j)
    punpckhbw xmm10,xmm0  ; xmm10 = (i-2,j+1), (i-2,j+2)

    paddw xmm1,xmm2     ; xmm1 = (i,j)+(i+1,j+1)  ||  (i,j+1)+(i+1,j+2)
    paddw xmm1,xmm3     ; xmm1 = (i,j)+(i+1,j+1)+(i+2,j+2)  ||  (i,j+1)+(i+1,j+2)+(i+2,j+3)
    paddw xmm1,xmm4     ; xmm1 = (i,j)+(i+1,j+1)+(i+2,j+2)+(i-1,j-1)  ||  (i,j+1)+(i+1,j+2)+(i+2,j+3)+(i-1,j)
    paddw xmm1,xmm5     ; xmm1 = (i,j)+(i+1,j+1)+(i+2,j+2)+(i-1,j-1)+(i-2,j-1)  ||  (i,j+1)+(i+1,j+2)+(i+2,j+3)+(i-1,j)+(i-2,j)

    paddw xmm6,xmm7     ; xmm6 = (i,j+2)+(i+1,j+3)  ||  (i,j+3)+(i+1,j+4)
    paddw xmm6,xmm8     ; xmm6 = (i,j+2)+(i+1,j+3)+(i+2,j+4)  ||  (i,j+3)+(i+1,j+4)+(i+2,j+5)
    paddw xmm6,xmm9     ; xmm6 = (i,j+2)+(i+1,j+3)+(i+2,j+4)+(i-1,j+1)  ||  (i,j+3)+(i+1,j+4)+(i+2,j+5)+(i-1,j+2)
    paddw xmm6,xmm10    ; xmm6 = (i,j+2)+(i+1,j+3)+(i+2,j+4)+(i-1,j+1)+(i-2,j+1)  ||  (i,j+3)+(i+1,j+4)+(i+2,j+5)+(i-1,j+2)+(i-2,j+2)

    movdqu xmm2,xmm1        ; xmm2 = (i,j)+(i+1,j+1)+(i+2,j+2)+(i-1,j-1)+(i-2,j-1)   ||  (i,j+1)+(i+1,j+2)+(i+2,j+3)+(i-1,j)+(i-2,j)
    movdqu xmm7,xmm6        ; xmm7 = (i,j+2)+(i+1,j+3)+(i+2,j+4)+(i-1,j+1)+(i-2,j+1) ||  (i,j+3)+(i+1,j+4)+(i+2,j+5)+(i-1,j+2)+(i-2,j+2)

    punpcklwd xmm1,xmm0     ; xmm1 = (i,j)+(i+1,j+1)+(i+2,j+2)+(i-1,j-1)+(i-2,j-1)
    punpckhwd xmm2,xmm0     ; xmm2 = (i,j+1)+(i+1,j+2)+(i+2,j+3)+(i-1,j)+(i-2,j)

    punpcklwd xmm6,xmm0     ; xmm6 = (i,j+2)+(i+1,j+3)+(i+2,j+4)+(i-1,j+1)+(i-2,j+1)
    punpckhwd xmm7,xmm0     ; xmm7 = (i,j+3)+(i+1,j+4)+(i+2,j+5)+(i-1,j+2)+(i-2,j+2)

    cvtdq2ps xmm1,xmm1  
    cvtdq2ps xmm2,xmm2  
    cvtdq2ps xmm6,xmm6  
    cvtdq2ps xmm7,xmm7

    movdqu xmm15,[cerocomados]

    mulps xmm1,xmm15
    mulps xmm2,xmm15
    mulps xmm6,xmm15
    mulps xmm7,xmm15

    cvtps2dq xmm1,xmm1
    cvtps2dq xmm2,xmm2
    cvtps2dq xmm6,xmm6
    cvtps2dq xmm7,xmm7

    packusdw xmm1,xmm2
    packusdw xmm6,xmm7

    packuswb xmm1,xmm6

    movdqu [rsi+rax],xmm1   

    add r11,4
    jmp .loop_x

.endloop_x:
    inc r10
    jmp .loop_y

.endloop_y:
    
    add rsp,8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret