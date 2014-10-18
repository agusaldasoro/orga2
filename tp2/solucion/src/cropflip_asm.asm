global cropflip_asm

section .text

;void tiles_asm(
;	unsigned char *src, RDI
;       unsigned char *dst, RSI
;	int cols, 	EDX 
;	int rows, 	ECX
;       int src_row_size, R8D
;       int dst_row_size, R9D
;	int tamx, 	R12d
;	int tamy, 	R13d
;	int offsetx, 	R14d 
;	int offsety, 	R15d

cropflip_asm:
	push rbp
	mov rbp,rsp
	push rbx
	push r12
	push r13
	push r14
	push r15
	sub rsp,8

	mov r12d, [rbp+16] ;tamx
	mov r13d, [rbp+24] ;tamy
	mov r14d, [rbp+32] ;offsetx
	mov r15d, [rbp+40] ;offsety

	mov r12d,r12d
	mov r13d,r13d
	mov r14d,r14d
	mov r15d,r15d

	mov r10,r15 ; y
	mov r11,r14 ; x

	mov rcx,r13 ; y2 = tamy
	dec rcx     ; y2 = tamy-1;

	add r13,r15 ; r13 = offsety+tamy
	
	mov rbx,r12
	add rbx,r14 ; rbx = offsetx+tamx
.loop_y:
	cmp r10,r13 ; y < offsety+tamy
	jge .endloop_y
	mov r11,r14 ; x = offsetx
;	mov rdx,r12 ; x2 = tamx
	mov rdx,0 ; x2 = 0
.loop_x:
	cmp r11,rbx ; x < offsetx+tamx
	jge .endloop_x

	mov rax,r10
	imul rax,r8
	lea rax,[rax+r11*4]

	movdqu xmm0,[rdi+rax]

;	pshufd xmm0, xmm0,0x1B ; 00 01 10 11

	mov rax,rcx
	imul rax,r9
	lea rax,[rax+rdx*4]

;	movdqu [rsi+rax-16],xmm0
	movdqu [rsi+rax],xmm0

	add r11,4
;	sub rdx,4
	add rdx,4
	jmp .loop_x
.endloop_x:
	inc r10
	dec rcx
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
