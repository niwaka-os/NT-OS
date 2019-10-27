#include "NT-OS.h"
//VRAMの番地には、0xa0000 + x + y*320でアクセス

//FATエントリ7168(0x1c00)
//FAT12のディレクトリのエントリ構造体
typedef struct file_entry{
	char	filename[8];
	char	extension[3];
	char	attr;
	short	Reserved;
	short	creation_time;
	short	creation_date;
	short   last_access_date;
	short	ignore_fat12;
	short	last_write_time;
	short	last_write_date;
	short	first_logical_cluster;
	int		file_size;
}FILE_ENTRY;

//asmfunc.c
void write_mem8(int addr, int data);
void sti();
//GDT_IDT.
void idt_init(idt_register);
//floppy.c
void issue_command_read(char cylinder,char head, char sector);
void issue_command_write(char cylinder,char head, char sector);
void read_lba(int lba);
void write_lba(int lba);
//font.c
void font_init();
//console.c
void console_init(CONSOLE *c);

//メイン関数
void NTMAIN(){

	CONSOLE *cons;
	cons = 0x100000;	//0x10006~0x102000

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

	//for(i=0xfd000000 ; i < 0xfd0c0000; i++){
	//	*((char*)i) = 0;
	//}

	//FDからデータを読み込む。
	//シリンダ番号0、ヘッド番号0、セクタ番号1
	//issue_command_read(0, 0, 1);
	//print_font_ascii(65, 0, 0);
	//LBA方式で指定する。
	//read_lba(19);
	font_init();
	console_init(cons);

	//VRAMの番地には、0xa0000 + x + y*320でアクセス
	//print_font(&prompt, 0, 0);
    //print_font_ascii(65, 0, 0);
	//print_font_ascii(70, 60, 700);
	char s[]="FILE";
	//char s2[]="adf";

	//char s[] = "MN";
	//printf(s, 100, 0, 0);

	//rename(file_entry, (file_entry+1)->filename, "NNNNNNNNNNN");
	//search_directry_fat12(file_entry);	//lsコマンドみたいなもの

	//無限ループ
	for(;;){

	}
}

//文字列の長さを測る
unsigned int len(char* s){
        int i=0;
        int sum=0;

        for(i=0;;i++){
                if(s[i]==0){
                        break;
                }
                sum++;
        }

        return sum;
}

//printfまがいの関数
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

void ls(struct FILE* directory_entry){
	return;
}

//FAT32フォーマット上のlsコマンド使用
void search_directry_fat32(){
	return;
}

void search_directry_fat12(FILE_ENTRY* entry){

	//カウンタ変数
	int i, j, k;
	int entry_num=2;
	int str_len;

	FILE_ENTRY *entry_now;

	k=0;	
	//線形探索
	for(i=0; i < entry_num; i++){
		entry_now = (entry+i);	//今、注目しているエントリの番地をentry_nowに入れる
		//ファイルがあった。
		if(entry_now->filename[0] != 0){		
			//printf(&entry_now->filename, 0+(i*160), 20, 1);	//entry_now->filenameの番地を入れる
			//print_font_ascii(char ascii_code, int x, int y)
			char *str;
			str = &entry_now->filename;
			str_len = len(str);

			for(j=0; j < str_len; j++){
				if(*(str+j) == 32){	//スペースキーの場合、描画しない。
					continue;
				}else{
					print_font_ascii(*(str+j), 0+(k*8), 20);
					k++;
				}
			}
		}
	}

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