#include "NT-OS.h"

//void printf(char *font, int x, int y, int arg);
void print_font(char *font, int x, int y);
void print_font_ascii(char ascii_code, int x, int y);
int strcmp(char *s1, char *s2);
unsigned int len(char* s);
//keyboard.c
char pop_buff(BUFF *bf);


typedef struct vim_data{
    char filenmae[8];       //vimで使用するファイル名
    char vim_buf[1024*768]; //vimで使用できるテキストのバッファ
    char char command[10];  //vimコマンド
    int vram_start;         //vramの開始番地
    int vram_end;           //vramの終了番地
    int vram_high;          //画面の高さ
    int vram_side;          //画面の横
    int now_row, now_col;   //今、何行何列にいるのか?
    int max_row, max_col;   //最大何行何列
    int text_row, text_col; //テキスト領域の中での、今何行何列にいるのか?
    int text_max_row, text_max_col;//テキストエリアの最大何行何列
}VIM;

//keycode変換用、各要素は、keycodeに対応したASCII_CODEが格納されています。
char key_table[52]={0x00, 0x00, '1', '2', '3', '4', '5', '6', 0x37, 0x38, 0x39, 0x30,0x00, 0x00, 0x00, 0x00, 0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x5b,0x4f, 0x50, 0x40, 0x7b, 0x1c, 0x1d, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x3b, 0x3a, 0x00, 0x2a, 0x7d, 0x5a, 0x58, 0x43, 0x56,0x42, 0x4e, 0x4d};

//consoleのメイン関数
void console_main(CONSOLE *cons){
    BUFF *key_bf;
    key_bf = 0x00010010;
    unsigned char data;
	FILE_ENTRY* file_entry;
	file_entry = 0x3000;	//file_entryは0x3000番地から
    
    int flg;
    loop:
        while(1){
            //なぜかこのif文を入れないとバグが発生する。調査中
            if(key_bf->data_num >100){
                print_font_ascii(65, 30, 30);
            }
            if(key_bf->data_num>=1){
                //CLI命令ここで使う方がいいかも?
                data = pop_buff(key_bf);        //バッファからデータを取り出す。消費者
                cons->command[cons->command_num++]=key_table[data];          //commandにデータを送る。
                //STI命令で解除
                //Enterを押したとき
                if(data == 0x1c){
                    //コマンドチェックを行う。
                    flg = check_command(cons);
                    //コマンドのリセット
                    reset_command(cons);
                    //LBA方式でフロッピーディスクからデータを読み込みする。
                    if(flg==0){
                        read_lba(19);//論理セクタ番号19以降には、ディレクトリ領域が存在する。
                        //lsコマンド
                        ls(file_entry, cons);
                    }
                    //新しいプロンプト生成する。
                    new_prompt(cons);
                    //parse
                    goto loop;      //while分の先頭へ
                }
                console_setbuf(cons, font_ASCII[key_table[data]], cons->now_col, cons->now_row);
                consbuf_to_vram(cons);
            }
        }
}

//コマンドの中身をリセットする。
void reset_command(CONSOLE *cons){
    //コマンド文字列の数を0にする。
    cons->command_num = 0;
    return;
}

//こまんとチェックをする。
int check_command(CONSOLE *cons){
    //コマンドチェックを行う。
    if(cons->command[0] == *"L" && cons->command[1] == *"S"){   //0x4cは"L"
        //コマンドの中身をリセットする。
        return 0;   //コマンド実行を行う。
    }
    if(cons->command[0] == *"V" && cons->command[1] == *"I" && cons->command[2] == *"M"){
        return 1;   //vimコマンドを実行する。
    }
    
    return -1;      //コマンドは実行不可
}

//新しいプロンプトを生成する。
void new_prompt(CONSOLE *cons){
    cons->now_row++;
    cons->now_col=0;
    //最終列まで来た時
    if(cons->now_row >= cons->max_row){
        //コンソールリセットをする。
        console_reset(cons);
        return;
    }
    console_setbuf(cons, font_ASCII[78], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[65], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[79], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[83], cons->now_col, cons->now_row); 
    console_setbuf(cons, prompt, cons->now_col, cons->now_row);

    consbuf_to_vram(cons);
    return;
}
//commandにデータを送る。
void send_command(CONSOLE *cons){
    cons->command[cons->command_num++];
    return;
}

//コマンドの構文解析をする。
void parse_command(CONSOLE *cons){
    return;
}   

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
    cons->command_num = 0;
    int i;

    //バッファの初期化を行う。
    for(i=0; i < (cons->vram_high)*(cons->vram_side); i++){
        cons->print_buf[i] = 0;
    }
    
    console_print(cons);

    return;
}

//consoleのバッファ領域に１文字だけかく
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
		}
		if((data & 0x40) != 0){
			cons->print_buf[x+1+1024*(i+y)] = 15;
		}
		if((data & 0x20) != 0){
			cons->print_buf[x+2+1024*(i+y)] = 15;
		}
		if((data & 0x10)  != 0){
			cons->print_buf[x+3+1024*(i+y)] = 15;
		}
		if((data & 0x08) != 0){
			cons->print_buf[x+4+1024*(i+y)] = 15;
		}
		if((data & 0x04) != 0){
			cons->print_buf[x+5+1024*(i+y)] = 15;
		}
		if((data & 0x02) != 0){
			cons->print_buf[x+6+1024*(i+y)] = 15;
		}
		if((data & 0x01) != 0){
			cons->print_buf[x+7+1024*(i+y)] = 15;
		}
	}
    
    cons->now_col++;
    //最後の列まできた
    if(cons->now_col == cons->max_col){
        cons->now_col = 0;
        cons->now_row++;        
        if(cons->now_row >= cons->max_row){ //最後の行まできたs
            cons->now_row = 0;
            console_reset(cons);
        }
    }
}

//consoleの文字を１文字だけ削除する。
void delete_char_cons(CONSOLE *cons){
    return;
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
    for(i=cons->vram_start; i < cons->vram_end; i++){
        *((char*)i) = cons->print_buf[j];
        j++;
    }
    return;
}

//最大行まできたら、リセットする。
void console_reset(CONSOLE *cons){
    int i;
    int j;
    cons->now_col = 0;
    cons->now_row = 0;
    for(i = cons->vram_start; i < cons->vram_end; i++){
        cons->print_buf[j] = 0;
        j++;
    }
    console_setbuf(cons, font_ASCII[78], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[65], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[79], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[83], cons->now_col, cons->now_row); 
    console_setbuf(cons, prompt, cons->now_col, cons->now_row);

    consbuf_to_vram(cons);
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


//ファイル作成
void make_file(){
    return;
}

//ディレクトリ作成
void make_dir(){
    return;
}

//ファイル削除
void delete_file(){
    return;
}

//ディレクトリ削除
void delete_dir(){
    return;
}

//ファイル保存
void save_file(){
    return;
}

//vimプログラム
void vim_main(CONSOLE *cons, VIM *vim){
    //テキストエディタを立ち上げる

    //コンソールのリセットを行う。
    console_reset(cons);
    //vim用の画面を立ち上げる。
    startup_vim();
    //mainループ
    for(;;){

    }

    //vimを終了するので、コンソールを立ち上げる。
    console_reset(cons);
    return; //vimコマンド終了する。
}

//vimの画面を立ち上げる。
void startup_vim(CONSOLE *cons, VIM *vim){
    
    return;
}


//vimのバッファに描画データを送る。
void vim_setbuf(CONSOLE *cons, VIM *vim){

    return;
}


void ls(FILE_ENTRY* entry, CONSOLE*cons){

    cons->now_row++;
    cons->now_col=0;
    //最終行まで来た時
    if(cons->now_row >= cons->max_row){
        console_reset(cons);
    }

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
                console_setbuf(cons, font_ASCII[*(str+j)], cons->now_col, cons->now_row);
					k++;
				}
			}
		}
	}
                                
    consbuf_to_vram(cons);
}