#include "NT-OS.h"
#define VRAM_ADR 0xa0000		//VRAMの開始番地


//VRAMの番地には、0xa0000 + x + y*320でアクセス

//asmfunc.c
void write_mem8(int addr, int data);
void sti();
//GDT_IDT.
void idt_init(idt_register);
//floppy.c
void issue_command_read();
void issue_command_write();
void font_init();
//メイン関数
void NTMAIN(){

	IDTR *idt_register;
	idt_register =  0x10000;	//IDTR型へのポインタ自体の番地は、0x7c00番地に格納する。

	//IDT初期化する。
	idt_init(idt_register);
	//PIC初期化する。
	init_pic();
	//IDTの手続きが完了したので、割り込み再開する。
	sti();
	out_8(PIC0_IMR, 0xbf);
	//カウンタ変数
	int i;
	char data;

	//FDからデータを読み込む。
	issue_command_read();
	font_init();
	//VRAMの番地には、0xa0000 + x + y*320でアクセス
	print_font(&prompt, 0, 0);
	print_font_ascii(65, 10, 0);
	print_font_ascii(66, 20, 0);
	print_font_ascii(67, 30, 0);
	print_font_ascii(68, 40, 0);
	print_font_ascii(69, 50, 0);
	print_font_ascii(70, 60, 0);
	print_font_ascii(71, 70, 0);
	print_font_ascii(72, 80, 0);
	print_font_ascii(73, 90, 0);
	print_font_ascii(74, 100,0);
	print_font_ascii(75, 110,0);
	print_font_ascii(76, 120,0);
	print_font_ascii(77, 130,0);
	print_font_ascii(78, 140,0);
	print_font_ascii(79, 150,0);
	print_font_ascii(80, 160,0);
	//print_font_ascii(81, 170,0);
	//print_font_ascii(70, 60, 0);
	char s[]="O";
	//printf(s);

	//無限ループ
	for(;;){

	}
}

//文字列の長さを測る
void strlen(char *s){
	return;
}

//printfまがいの関数
void printf(char *s){

	if(s[0]==65){
		print_font_ascii(65, 60, 0);
	}
	if(s[0]==66){
		print_font_ascii(66, 60, 0);
	}
	if(s[0]==70){
		print_font_ascii(70, 60, 0);
	}
	if(s[0]==71){
		print_font_ascii(71, 60, 0);
	}
	if(s[0]==72){
		print_font_ascii(72, 60, 0);
	}
	if(s[0]==73){
		print_font_ascii(73, 60, 0);
	}
	if(s[0]==79){
		print_font_ascii(79, 160, 0);
	}

	return;
}

//１文字分表示
//x, yは、それぞれx座標とy座標に対応する。
void print_font(char *font, int x, int y){

	int i;
	char data;

	char* vram;
	vram = (char *)0xa0000;	

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
	vram = (char *)0xa0000;	

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
