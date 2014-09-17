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
	PUSH RBP
	MOV RBP,RSP
	
	PUSH R12
	PUSH R13
	PUSH R14
	PUSH R15

	MOV R12d, [RBP+16]
	MOV R13d, [RBP+24]
	MOV R14d, [RBP+32]
	MOV R15d, [RBP+40]	
	MOV R10, RCX

	MOV R14D,R14D
	LEA RAX,[RDI+R14*4]

	MOV R8D,R8D
	MOV R15D,R15D
	imul r15, r8
	LEA RAX, [RAX+R15]

	MOV R11,RSI
	MOV R9D,R9D

	MOV R13D, R13D
	MOV RCX, R13

	MOV R12D, R12D
	.CICLO:
	MOV R9, RCX
	MOV RCX, R12
	SAR ecx,2
	dec ecx
	.CICLO2:
	MOVDQu XMM0, [RAX]
	PSHUFD XMM0, XMM0,0xE4
	MOVDQu [R11], XMM0


	LEA RAX, [RAX+16]
	LEA R11, [R11+16]
	;add r11,16															
	LOOP .CICLO2

	LEA  RAX, [RAX+R8]
	LEA R11,[R11+R9]
	MOV RCX, R9
	LOOP .CICLO
	
	POP R15
	POP R14
	POP R13
	POP R12
	POP RBP
    ret
