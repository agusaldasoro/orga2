
global mblur_asm

section .data

cerocomados dd 0.2,0.2,0.2,0.2
rgb_only    dd 0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF


section .text
;void mblur_asm    (
	;unsigned char *src, rdi
	;unsigned char *dst, rsi
	;int filas, edx
	;int cols, ecx
	;int src_row_size, r8d
	;int dst_row_size) r9d

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
	mov r10,0
	; sub edx,2
	; sub ecx,2
	xor r15,r15
	xor r14,r14
	mov r15d,edx
	mov r14d,ecx
	sub r15,2
	sub r14,2
	pxor xmm13,xmm13
.loop_y:
	cmp r10d,ecx ; y < filas
	jge .endloop_y
	mov r11,0
.loop_x:
	cmp r11d,edx ; x < cols
	jge .endloop_x

	mov rax,r10
	imul eax,r8d
	lea rax,[rax+r11*4]

	cmp r11,2
	jl .cero
	cmp r10,2 
	jl .cero
	cmp r10,r14
	jge .cero
	cmp r11,r15
	jge .cero


	jmp .nocero
.cero:
	movq [rsi+rax],xmm13		
	add r11,2
	jmp .loop_x

.nocero:
	; mov rax,r10
	; imul eax,r8d
	; lea rax,[rax+r11*4]

	lea rbx,[rdi+rax]
	movdqu xmm1,[rbx]        ; (i,j)
	movdqu xmm2,[rbx+r8*1+4] ; (i+1,j+1)
	movdqu xmm3,[rbx+r8*2+8] ; (i+2,j+2)
	mov r12,rbx
	sub r12,r8
	movdqu xmm4,[r12-4] ; (i-1,j-1)
	sub r12,r8
	movdqu xmm5,[r12-8] ; (i-2,j-1)

	movdqu xmm14,[rgb_only]

	; pand xmm1,xmm14
	; pand xmm2,xmm14
	; pand xmm3,xmm14
	; pand xmm4,xmm14
	; pand xmm5,xmm14

	pxor xmm0,xmm0
	movdqu xmm6,xmm1
	punpcklbw xmm1,xmm0 
	punpckhbw xmm6,xmm0

	movdqu xmm7,xmm2
	punpcklbw xmm2,xmm0
	punpckhbw xmm7,xmm0

	movdqu xmm8,xmm3
	punpcklbw xmm3,xmm0
	punpckhbw xmm8,xmm0

	movdqu xmm9,xmm4
	punpcklbw xmm4,xmm0
	punpckhbw xmm9,xmm0

	movdqu xmm10,xmm5
	punpcklbw xmm5,xmm0
	punpckhbw xmm10,xmm0

	paddw xmm1,xmm2
	paddw xmm1,xmm3
	paddw xmm1,xmm4
	paddw xmm1,xmm5

	paddw xmm6,xmm7
	paddw xmm6,xmm8
	paddw xmm6,xmm9
	paddw xmm6,xmm10

	movdqu xmm2,xmm1
	movdqu xmm7,xmm6

	punpcklwd xmm1,xmm0
	punpckhwd xmm2,xmm0

	punpcklwd xmm6,xmm0
	punpckhwd xmm7,xmm0

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

	;mov rax,r10
	;imul eax,r9d
	;lea rax,[rax+r11*4]
	;lea rbx,[rsi+rax]
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
 
