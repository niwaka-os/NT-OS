;プロテクトモードオンにする
bits 16
ORG 0xc200      ;このプログラムは、0xc200番地に読み込まれる

BOTPAK	EQU		0x00280000		; bootpackのロード先

MOV BX, 0x4105
MOV AX, 0x4f02
INT		0x10

;割り込み禁止する
CLI 

;A20 enable
MOV AL, 0xd1
OUT 0x64, AL
MOV AL, 0xdf
OUT 0x60, AL


GDT:
    LGDT [GDTR_INIT]
    ;下の三行を実行すると、プロテクトモードに入る。
    MOV EAX, CR0
	;AND		EAX,0x7fffffff	; bit31を0にする（ページング禁止のため）
    OR  EAX, 1
    MOV CR0, EAX
    JMP flash
flash:
    MOV AX, 0x08
    MOV DS, AX
    MOV ES, AX
    MOV FS, AX
    MOV GS, AX
    MOV SS, AX

;NT-MAINの転送

    MOV ESI, main   ;転送元 main label
    MOV EDI, BOTPAK ;転送先
    MOV ECX, 512*1024/4
    CALL memcpy

;NT-MAINの起動
    MOV EBX, BOTPAK
    MOV ECX, [EBX+16]
    ADD ECX, 3
    SHR ECX, 2
    JZ SKIP
    MOV ESI, [EBX+20]
    ADD ESI, EBX
    MOV EDI, [EBX+12]
    CALL memcpy

SKIP:
    MOV ESP, [EBX+12]   ;スタック初期値
    JMP DWORD 16:0x0000001b

GDT_INIT
    RESB 8
	DB 0xff, 0xff, 0x00, 0x00, 0x00, 0x92, 0xcf, 0x00   ;テスト
	DB 0xff, 0xff, 0x00, 0x00, 0x28, 0x9a, 0x47, 0x00   ;テスト
    ;DB 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x00   ;開始番地：0x0000番地   リミット値4GB
    ;DB 0x40, 0x42, 0x00, 0x90, 0x00, 0x9a, 0x4f, 0x00   ;開始番地：0x9000番地   リミット値1000KB   

GDTR_INIT:
    DW 23           ;GDTの大きさ-1
    DD GDT_INIT     ;GDT_INITのアドレス   

memcpy:
    MOV EAX, [ESI]
    ADD ESI, 4
    MOV [EDI], EAX
    ADD EDI, 4
    SUB ECX, 1
    JNZ memcpy
    RET

main: