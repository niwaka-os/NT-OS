bits 32
global io_hlt
global write_mem8
global lidt
global sti
global in_8
global out_8
global asm_inthandler26
global asm_inthandler21

extern inthandler26	;ここで使う。
extern inthandler21
section .text
io_hlt:
	HLT
	RET

write_mem8:
	MOV ECX, [ESP+4]
	MOV AL, [ESP+8]
	MOV [ECX], AL
	RET

lidt:   	;void lidt(short limit, int base)
	LIDT [ESP+4]
	RET

sti:		;void sti()
	STI
	RET

in_8:		;void in_8(int port);
	MOV DX, [ESP+4]
	IN  AL, DX
	RET

out_8:		;void out_8(int port, char value);
	MOV AL, [ESP+8]
	MOV DX, [ESP+4]
	OUT	DX, AL
	RET 

asm_inthandler21:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	inthandler21
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

asm_inthandler26:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	inthandler26
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD
