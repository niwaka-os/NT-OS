[bits 16]               ;リアルモードで動くので、16bitコード指定する(nasmのデフォは、16bitコードなので、指定しなくても別にいい。)
ORG		0x7c00			;アセンブラに対しての命令  0x7c00番地から読み込まれることを想定して、アセンブラにラベルの計算をしてもらう。


JMP		register_init   ;register_initにジャンプする
DB		0x90
DB		"TETSUYA "		; ブートセクタの名前を自由に書いてよい（8バイト）
DW		512				; 
DB		1				; クラスタの大きさ（1セクタにしなければいけない）
DW		1				; FATがどこから始まるか（普通は1セクタ目からにする）
DB		2				; FATの個数（2にしなければいけない）
DW		224				; ルートディレクトリ領域の大きさ（普通は224エントリにする）
DW		2880			; このドライブの大きさ（2880セクタにしなければいけない）
DB		0xf0			; メディアのタイプ（0xf0にしなければいけない）
DW		9				; FAT領域の長さ（9セクタにしなければいけない）
DW		18				; 1トラックにいくつのセクタがあるか（18にしなければいけない）
DW		2				; ヘッドの数（2にしなければいけない）
DD		0				; パーティションを使ってないのでここは必ず0
DD		2880			; このドライブ大きさをもう一度書く
DB		0,0,0x29		; よくわからないけどこの値にしておくといいらしい
DD		0xffffffff		; たぶんボリュームシリアル番号
name:
DB		"NT-OS      "	; ディスクの名前（11バイト）
DB		"FAT12   "		; フォーマットの名前（8バイト）
RESB	18				; とりあえず18バイトあけておく

;初期化
register_init:
    MOV AX, 0x00
    MOV ES, AX
    MOV SS, AX
    MOV SP, 0x7c00
    MOV DS, AX
    JMP mode

;読み込みモードを設定する
;AH(読み込みモード)=0x02、DH(ヘッド番号)=0, DL(ドライブ番号)=0, AL(処理するセクタ数)=1 
mode:
    MOV DH, 0   ;ヘッド番号
    MOV DL, 0   ;ドライブ番号
    MOV CH, 0   ;シリンダ番号
    MOV CL, 2   ;セクタ番号
    MOV BX, 0x7e00  ;バッファアドレス0x7e00番地から
    JMP read    ;readラベルに飛ぶ

;FDからデータを読み込んでいく
read:
    MOV AH, 0x02    ;読み込みモード
    MOV AL, 1       ;1セクタ分読み込む
    INT 0x13        ;割り込み番号13を呼び出す
    JC error_print  ;ソフトウェア割り込み失敗時、ジャンプする
    ADD BX, 0x200   ;512byte足す
    ADD CL, 0x01       ;セクタ番号を1足す
    CMP CL, 19      ;19とCL(セクタ番号を比較する)
    JE add_head     ;19と等しい場合、シリンダ番号とセクタ番号とヘッド番号の調整を行う。
    JMP read

add_head:
    MOV CL, 0x01       ;セクタ番号を1にする
    ADD DH, 0x01       ;ヘッド番号を1足す
    CMP DH, 0x02       ;ヘッド番号が2の場合、ラベルadd_cylinderで調整を行う。
    JE add_cylinder
    JMP read

add_cylinder:
    MOV DH, 0x00           ;ヘッド番号を0にする
    ADD CH, 0x01
    CMP CH, 0x05  ;シリンダ番号が5まできたら終わり
    JE print   ;シリンダ番号が、5のとき終了する
    JMP read

;処理成功した場合実行
print:
    MOV AX, 0
    MOV ES, AX
    MOV AH, 0x13
	MOV AL, 0x00
    MOV BH, 0x00
	MOV BL, 15
    MOV CX, 5
    MOV DL, 0x10
    MOV DH, 0x10
    MOV BP, msg_error
    INT 0x10
    JC error_print
	JMP fin

;処理失敗した場合実行
error_print:
    MOV AX, 0
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

msg_error:
    DB "error!!!"

msg_suc:
    DB "SUC!!"

RESB 0x1fe-($-$$)
DB 0x55, 0xaa
DB "HEllO!!!"   ;2セクタ
RESB 0x400-($-$$)
DB "HELLO!!!"   ;3セクタ
RESB 0x600-($-$$)
DB "HELLO!!!"   ;4セクタ
RESB 0x800-($-$$)
DB "HELLO!!!"   ;5セクタ
RESB 0xA00-($-$$)
DB "HELOO!!!"
RESB 0xC00-($-$$)
DB "HELLO!!!"  
RESB 0x2400-($-$$)
DB "HELLO!!!"              ;1シリンダ、1セクタ
RESB 0x167FF-($-$$)
msg:
    DB "H"