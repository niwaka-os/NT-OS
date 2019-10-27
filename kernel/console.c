#include "NT-OS.h"

//void printf(char *font, int x, int y, int arg);
void print_font(char *font, int x, int y);
void print_font_ascii(char ascii_code, int x, int y);
int strcmp(char *s1, char *s2);

//コンソール初期をする。
void console_init(CONSOLE *cons){
    cons->x=0;
    cons->y=0;
    cons->now_col=0;
    cons->now_row=0;
    cons->max_col=102;
    cons->max_row=48;
    cons->vram_start = VRAM_ADR;
    cons->vram_end   = 0xfd0c0000;
    cons->vram_high  = 1024;
    cons->vram_side  = 768;
    int i;

    //バッファの初期化を行う。
    for(i=0; i < cons->vram_high*cons->vram_side; i++){
        cons->print_buf[i] = 0;
    }
    
    console_print(cons);

    return;
}

//consoleのバッファ領域に描画データをかく
void console_setbuf(CONSOLE *cons, char *font, int col, int row){
	char data;
    int i;
    int x, y;
    
    x = 10*col;
    y = 16*row;

    for(i=0; i < 16; i++){
		data = font[i];

		if((data & 0x80) != 0){
			cons->print_buf[x+1024*(i+y)] = 15;
			//write_mem8(0xa0000+0+320*i, 15);
		}
		if((data & 0x40) != 0){
			cons->print_buf[x+1+1024*(i+y)] = 15;
			//write_mem8(0xa0000+1+320*i, 15);
		}
		if((data & 0x20) != 0){
			cons->print_buf[x+2+1024*(i+y)] = 15;
			//write_mem8(0xa0000+2+320*i, 15);
		}
		if((data & 0x10)  != 0){
			cons->print_buf[x+3+1024*(i+y)] = 15;
			//write_mem8(0xa0000+3+320*i, 15);
		}
		if((data & 0x08) != 0){
			cons->print_buf[x+4+1024*(i+y)] = 15;
			//write_mem8(0xa0000+4+320*i, 15);
		}
		if((data & 0x04) != 0){
			cons->print_buf[x+5+1024*(i+y)] = 15;
			//write_mem8(0xa0000+5+320*i, 15);
		}
		if((data & 0x02) != 0){
			cons->print_buf[x+6+1024*(i+y)] = 15;
			//write_mem8(0xa0000+6+320*i, 15);
		}
		if((data & 0x01) != 0){
			cons->print_buf[x+7+1024*(i+y)] = 15;
			//write_mem8(0xa0000+7+320*i, 15);
		}
	}
    
    cons->now_col++;
    //最後の列まできた
    if(cons->now_col == cons->max_col){
        cons->now_col = 0;
        cons->now_row++;        
        if(cons->now_row == cons->max_row){ //最後の行まできたs
            cons->now_row = 0;
            console_reset(cons);
        }
    }
}

//コンソール画面を描画する。
void console_print(CONSOLE *cons){
    console_setbuf(cons, font_ASCII[78], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[65], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[79], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[83], cons->now_col, cons->now_row); 
    console_setbuf(cons, prompt, cons->now_col, cons->now_row);

    consbuf_to_vram(cons);

    return;
}

//コンソールのバッファからVRAMへ
void consbuf_to_vram(CONSOLE *cons){
    int i;
    int j=0;
    for(i=cons->vram_start; i < 0xfd0c0000; i++){
        *((char*)i) = cons->print_buf[j];
        j++;
    }
    return;
}

//最大行まできたら、リセットする。
void console_reset(CONSOLE *cons){
    int i;
    int j;
    for(i = cons->vram_start; i < cons->vram_end; i++){
        cons->print_buf[j] = 0;
        j++;
    }
    console_setbuf(cons, font_ASCII[78], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[65], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[79], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[83], cons->now_col, cons->now_row); 
    console_setbuf(cons, prompt, cons->now_col, cons->now_row);
    return;
}

//指定した範囲(行))のコンソール画面をリセットをする。
void console_subreset(CONSOLE *cons, int row){
    return;
}

char prompt[16] ={
    0x00,
    0x40,
    0x20,
    0x10,
    0x08,
    0x04,
    0x02,
    0x01,
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x40,
    0x00,
};


