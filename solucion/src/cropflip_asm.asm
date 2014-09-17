global cropflip_asm

section .text

;	void tiles_asm(unsigned char *src, RDI
;       unsigned char *dst, RSI
;	int cols EDX, rows R10
;       int src_row_size, R8D
;       int dst_row_size, R9D
;	int tamx 16 R12, int tamy R13,
;	int offsetx 48 R14, int offsety) 64 R15;

cropflip_asm:
	push rbp
	mov rbp,rsp
	
	push rdi
	push rsi
	push r12
	push r13
	push r14
	push r15

	xor r12,r12
	xor r13,r13
	xor r14,r14
	xor r15,r15

	mov r12d, [rbp+16]
	mov r13d, [rbp+24]
	mov r14d, [rbp+32]
	mov r15d, [rbp+40]


	mov r12d,r12d
	mov r13d,r13d
	mov r14d,r14d
	mov r15d,r15d

	mov r10,r15 ; y
	mov r11,r14 ; x

	dec r13    ; y2 = tamy-1;
	add r13,r15 ; offsety+tamy
	mov rbx,r12
	add rbx,r14 ; offsetx+tamx
.loop_y:
	; cmp r10,offsety+tamy
	cmp r10,r13 ; offsety+tamy
	jge .endloop_y
	mov r11,r14 
.loop_x:
	; mov rbx,r14
	; add rbx,r12
	cmp r11,rbx ; offsetx+tamx
	jge .endloop_x

	xor rax,rax
	mov rax,r10
	mul r8
	lea rax,[rax+r11*4]
	movdqu xmm0,[rdi+rax]

	;mov rax,r15
	;mul r8
	;add rax,rdi
	; lea rax,[rax+r11*4] ; si maxi, ya sé

	; pshufd xmm0, xmm0,0xE4

	mov rax,r
	mul r9
	lea rax,[rax+r11*4]
	movdqu [rsi+rax],xmm0

	add r11,4
	jmp .loop_x
.endloop_x:
	inc r10
	jmp .loop_y
.endloop_y:
	
	; mov R10, RCX
	; mov R14D,R14D
	; LEA RAX,[RDI+R14*4]

	; mov r8d,r8d
	; mov r15d,r15d
	; dec r15
	; imul r15, r8
	; LEA RAX, [RAX+R15]

	; mov R11,RSI
	; mov R9D,R9D

	; mov R13D, R13D
	; mov RCX, R13

	; mov R12D, R12D
	; .CICLO:
	; mov R9, RCX
	; mov RCX, R12
	; SAR ecx,2
	; dec ecx
	; .CICLO2:
	; movDQu XMM0, [RAX]
	; PSHUFD XMM0, XMM0,0xE4
	; movDQu [R11], XMM0


	; LEA RAX, [RAX+16]
	; ; LEA R11, [R11+16]
	; add r11,16															
	; LOOP .CICLO2

	; LEA  RAX, [RAX+R8]
	; LEA R11,[R11+R9]
	; mov RCX, R9
	; LOOP .CICLO



	


	pop R15
	pop R14
	pop R13
	pop R12
	pop rsi
	pop rdi
	pop rbp
    ret
