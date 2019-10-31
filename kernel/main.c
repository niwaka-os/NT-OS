#include "NT-OS.h"

//VRAMの番地には、0xa0000 + x + y*320でアクセス


//asmfunc.c
void write_mem8(int addr, int data);
void sti();
//GDT_IDT.
void idt_init(idt_register);
//floppy.c
void issue_command_read(char cylinder,char head, char sector);
void issue_command_write(char cylinder,char head, char sector);
void read_lba(int lba);
void write_lba(int lba, char *buf);
//font.c
void font_init();
//console.c
void console_init(CONSOLE *c);

//メイン関数
void NTMAIN(){

	CONSOLE *cons;
	cons = 0x100000;	//0x100000~0x1fffff
	//90000
	//fffffぐらい必要
	//500000

	FILE_ENTRY* file_entry;
	file_entry = 0x3000;	//file_entryは0x3000番地から
	

	//0x10000番地から、48bit分
	IDTR *idt_register;
	idt_register =  0x10000;

	//keyboard.cの関数
	init_buf();

	//IDT初期化する。
	idt_init(idt_register);
	//PIC初期化する。

	init_pic();
	//init_kbc();
	//IDTの手続きが完了したので、割り込み再開する。
	sti();
	out_8(PIC0_IMR, 0xbd);
	//カウンタ変数
	int i;
	char data;

	//FDからデータを読み込む。
	//シリンダ番号0、ヘッド番号0、セクタ番号1
	//issue_command_read(0, 0, 1);
	//print_font_ascii(65, 0, 0);
	//LBA方式で指定する。
	//read_lba(19);	console.cの方で実行するから、ここでは隠す。
	font_init();
	read_fat12region();//FAT領域を読み出す。
	read_fat12dir_region();
	console_init(cons);
	//rename(file_entry, (file_entry+1)->filename, "NNNNNNNNNNN");
	//search_directry_fat12(file_entry);	//lsコマンドみたいなもの
	//コンソールのメイン関数
	console_main(cons);

	//無限ループ
	for(;;){

	}
}

//文字列の長さを測る
unsigned int len(char* s){
        int i=0;
        int sum=0;

        for(i=0;;i++){
                if(s[i]=='\0'){
                        break;
                }
                sum++;
        }

        return sum;
}

//printfまがいの関数
/***
void printf(char *font, int x, int y, int arg){
	int strlen, i, j;
	strlen = len(font);

	j=0;//実際に描画すべき位置
	for(i=0;i<strlen;i++){
		if(font[i]==*"%"){
			print_font_ascii(48+arg, x+(10)*j, y);
			i+=1;
			j++;
			if(i>=strlen){
				break;
			}
			continue;
		}
			print_font_ascii(*(font+i), x+(10)*j, y);
			j++;
	}
}
***/
//sprintf();

//１文字分表示  ascii_codeに対応していないフォントを描画する。
//x, yは、それぞれx座標とy座標に対応する。
void print_font(char *font, int x, int y){

	int i;
	char data;

	char* vram;
	vram = (char *)VRAM_ADR;	

	for(i=0; i < 16; i++){
		data = font[i];

		if((data & 0x80) != 0){
			vram[x+1024*(i+y)] = 15;
			//write_mem8(0xa0000+0+320*i, 15);
		}
		if((data & 0x40) != 0){
			vram[x+1+1024*(i+y)] = 15;
			//write_mem8(0xa0000+1+320*i, 15);
		}
		if((data & 0x20) != 0){
			vram[x+2+1024*(i+y)] = 15;
			//write_mem8(0xa0000+2+320*i, 15);
		}
		if((data & 0x10)  != 0){
			vram[x+3+1024*(i+y)] = 15;
			//write_mem8(0xa0000+3+320*i, 15);
		}
		if((data & 0x08) != 0){
			vram[x+4+1024*(i+y)] = 15;
			//write_mem8(0xa0000+4+320*i, 15);
		}
		if((data & 0x04) != 0){
			vram[x+5+1024*(i+y)] = 15;
			//write_mem8(0xa0000+5+320*i, 15);
		}
		if((data & 0x02) != 0){
			vram[x+6+1024*(i+y)] = 15;
			//write_mem8(0xa0000+6+320*i, 15);
		}
		if((data & 0x01) != 0){
			vram[x+7+1024*(i+y)] = 15;
			//write_mem8(0xa0000+7+320*i, 15);
		}
	}
}

void print_font_ascii(char ascii_code, int x, int y){

	int i;
	char data;

	char* vram;
	vram = (char *)VRAM_ADR;	

	for(i=0; i < 16; i++){
		data = font_ASCII[ascii_code][i];

		if((data & 0x80) != 0){
			vram[x+1024*(i+y)] = 15;
		}
		if((data & 0x40) != 0){
			vram[x+1+1024*(i+y)] = 15;
		}
		if((data & 0x20) != 0){
			vram[x+2+1024*(i+y)] = 15;
		}
		if((data & 0x10)  != 0){
			vram[x+3+1024*(i+y)] = 15;
		}
		if((data & 0x08) != 0){
			vram[x+4+1024*(i+y)] = 15;
		}
		if((data & 0x04) != 0){
			vram[x+5+1024*(i+y)] = 15;
		}
		if((data & 0x02) != 0){
			vram[x+6+1024*(i+y)] = 15;
		}
		if((data & 0x01) != 0){
			vram[x+7+1024*(i+y)] = 15;
		}
	}
	return;
}

//文字列の長さを比較する。
int strcmp(char *s1, char *s2){
	int len1;
	int len2;

	len1 = len(s1);
	len2 = len(s2);

	if(len1 > len2){	
		return 1;
	}
	else if(len1 == len2){	
		return 0;
	}
	
	return -1;
}

//FAT32フォーマット上のlsコマンド使用
void search_directry_fat32(){
	return;
}

//ファイルの名前を変更する。
void rename(FILE_ENTRY *entry, char *name_from, char *name_to){
	
	int i, j;//カウンタ変数
	int entry_num = 2;
	FILE_ENTRY *now_entry;	//今注目しているエントリー
	int str_len;
	//線形探索
	for(i=0; i < entry_num; i++){
		now_entry = (entry+i);

		//合致する名前の検索
		if(cmp_name(&now_entry->filename, name_from)==1){
			str_len = len(now_entry->filename);
			for(j=0; j < str_len; j++){
				now_entry->filename[j] = name_to[j];
			}
			print_font_ascii(65, 10, 0);
		}
	}

}

//ファイルの名前を比べる。
int cmp_name(char *s1, char *s2){
	
	//そもそも名前の長さが違う。
	if(len(s1) != len(s2)){
		return -1;
	}

	int i;//カウンタ変数
	for(i=0; i < len(s1); i++){
		if(s1[i]!=s2[i]){
			return -1;
		}
	}

	//合致した。
	return 1;
}

//四角形を描画する。
void draw_box(){
	return;
}

//三角形を描画する。
void draw_tryangle(){
	return;
}