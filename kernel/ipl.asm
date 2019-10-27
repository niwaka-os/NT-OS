bits 16               
ORG		0x7c00			  ;アセンブラに対しての命令  

SECTORS   EQU 19          ;実際は、18だが、条件式の都合により1プラスしてる。
CYLINDERS EQU 10          ;
HEADS     EQU 2           ;ヘッドの数


JMP		register_init   ;register_initにジャンプする
DB		0x90
DB		"TETSUYA "		
DW		512				 
DB		1				
DW		1			
DB		2			
DW		224			
DW		2880		
DB		0xf0		
DW		9			
DW		18			
DW		2			
DD		0			
DD		2880		
DB		0,0,0x29		
DD		0xffffffff	
DB		"NT-OS      "	
DB		"FAT12   "	
RESB	18				

;初期化
register_init:
    MOV AX, 0x00
    MOV BX, 0           ;BXアドレスは0番地で固定する
    MOV SS, AX
    MOV SP, 0x7c00
    MOV DS, AX
    MOV AX, 0x0820      ;バッファアドレスの開始番地を0x8200番地に設定する
    MOV ES, AX
    JMP mode


;読み込みモードを設定する
;AH(読み込みモード)=0x02、DH(ヘッド番号)=0, DL(ドライブ番号)=0, AL(処理するセクタ数)=1 
mode:
    MOV DH, 0  ;ヘッド番号
    MOV DL, 0   ;ドライブ番号
    MOV CH, 0   ;シリンダ番号
    MOV CL, 2  ;セクタ番号
    JMP read    ;readラベルに飛ぶ                 

;FDからデータを読み込んでいく
read:
    MOV AH, 0x02    ;読み込みモード
    MOV AL, 0x01       ;1セクタ分読み込む
    INT 0x13        ;割り込み番号13を呼び出す
    JC error_print  ;ソフトウェア割り込み失敗時、ジャンプする
    MOV AX, ES      ;バッファアドレス1足す
    ADD AX, 0x20
    MOV ES, AX
    ADD CL, 1            ;セクタ番号を1足す
    CMP CL, SECTORS      ;20とCL(セクタ番号を比較する)
    JE manage_head       ;20と等しい場合、シリンダ番号とセクタ番号とヘッド番号の調整を行う。  JE:::A==Bのとき、ラベルに示すアドレスにジャンプする。
    JMP read

manage_head:
    MOV CL, 0x01       ;セクタ番号を1にする
    ADD DH, 0x01       ;ヘッド番号を1足す
    CMP DH, HEADS      ;ヘッド番号が2の場合、ラベルadd_cylinderで調整を行う。
    JE manage_cylinder
    JMP read

manage_cylinder:
    MOV DH, 0x00        ;ヘッド番号を0にする
    ADD CH, 0x01
    CMP CH, CYLINDERS   ;シリンダ番号が5まできたら終わり
    JE print            ;シリンダ番号が、5のとき終了する
    JMP read

;処理成功した場合実行
print:
    JMP video_mode
	
video_mode:
    JMP 0xc200      ;0xc200番地へジャンプする
    JMP fin

;処理失敗した場合実行
error_print:
    MOV AX, 0x00
    MOV ES, AX
    MOV AH, 0x13
	MOV AL, 0x00
    MOV BH, 0x00
	MOV BL, 15
    MOV CX, 8
    MOV DL, 0x10
    MOV DH, 0x10
    MOV BP, msg_error
    INT 0x10
	JMP fin

;処理完了
fin:
    HLT
    JMP fin

;DBとDWの違い実験
;DB 0x12, 0x23
;DW 0x1223

;エラーメッセージ
msg_error:
    DB "error!!!"

RESB 0x1fc-($-$$)
DB 0x32, 0x43   ;DMA確認用
DB 0x55, 0xaa



