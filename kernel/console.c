#include "NT-OS.h"

//void printf(char *font, int x, int y, int arg);
void print_font(char *font, int x, int y);
void print_font_ascii(char ascii_code, int x, int y);
int strcmp(char *s1, char *s2);

int cmp_name(char *s1, char *s2);
//keyboard.c
char pop_buff(BUFF *bf);

char prompt[16] ={0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x00,};

//VIM用のファイルは、1024*768以上に大きくならない。
typedef struct vim_data{
    char filename[11];       //vimで使用するファイル名
    char vim_buf[1024*768]; //vimで使用できるテキストのバッファ
    char command[10];  //vimコマンド
    int vram_start;         //vramの開始番地
    int vram_end;           //vramの終了番地
    int vram_high;          //画面の高さ
    int vram_side;          //画面の横
    int now_row, now_col;   //今、何行何列にいるのか?
    int max_row, max_col;   //最大何行何列
    int text_row, text_col; //テキスト領域の中での、今何行何列にいるのか?
    int text_max_row, text_max_col;//テキストエリアの最大何行何列
    char vim_file[1024*768];  //ファイル保存用エリア。
    int vim_file_num;     
}VIM;



//keycode変換用、各要素は、keycodeに対応したASCII_CODEが格納されています。
char key_table[54]={0x00, 0x00, '1', '2', '3', '4', '5', '6', 0x37, 0x38, 0x39, 0x30,0x2d, 0x00, 0x00, 0x00, 0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49,0x4f, 0x50, 0x40, 0x7b, 0x1c, 0x1d, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x2b, 0x3a, 0x00, 0x2a, 0x7d, 0x5a, 0x58, 0x43, 0x56,0x42, 0x4e, 0x4d, 0x00, 0x2e};

//consoleのメイン関数
void console_main(CONSOLE *cons){
    BUFF *key_bf;
    key_bf = 0x00010010;
    unsigned char data;
	FILE_ENTRY* file_entry;

    file_entry = fat12_entry;
    //500000
    VIM *vim;
    vim = 0x500000;         //0x500000~0x5fffffは、vim用の領域

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
                //STI命令で解除しようから?
                //Enterを押したとき
                if(data == 0x1c){
                    //コマンドチェックを行う。
                    flg = check_command(cons);

                    //LBA方式でフロッピーディスクからデータを読み込みする。
                    if(flg==0){//lsコマンドを実行する。
                        read_lba(19);//論理セクタ番号19以降には、ディレクトリ領域が存在する。
                        //lsコマンド
                        ls(file_entry, cons);
                    }else if(flg==1){//vimコマンド実行する。
                        //ファイル名を求める。
                        vim_main(vim, cons);
                    }
                    //新しいプロンプト生成する。
                    new_prompt(cons);
                    //parse
                    reset_command(cons);
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
    int i;
    for(i=0; i < 100; i++){
        cons->command[i] = 0x00;
    }
    return;
}

//こまんとチェックをする。
int check_command(CONSOLE *cons){
    int len;
    len = cons->command_num;
    cons->command[len-1] = '\0';       //コマンド

    char LS_COMMAND[]="LS";           //LSコマンド
    char VIM_COMMAND[]="VIM";         //VIMコマンド

    //コマンドチェックを行う。
    if(cmp_name(&LS_COMMAND[0], &cons->command[0])==1){   
        return 0;   //コマンド実行を行う。
    }
    else if(cmp_name(&VIM_COMMAND[0], &cons->command[0])==1){
        return 1;   //vimコマンドを実行する。
    }

    return -1;      //コマンドは実行不可
}

//
void len_nt(char *s){

        int i=0;
        int sum=0;

        for(i=0;;i++){
                if(s[i]=='\0'){
                        break;
                }
                sum++;
        }

        return sum;

        return;
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
    console_setbuf(cons, font_ASCII[0x4e], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[0x54], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[0x2d], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[0x4f], cons->now_col, cons->now_row); 
    console_setbuf(cons, font_ASCII[0x53], cons->now_col, cons->now_row); 

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

//コンソール初期化を行う。
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
    console_setbuf(cons, font_ASCII[0x4e], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[0x54], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[0x2d], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[0x4f], cons->now_col, cons->now_row); 
    console_setbuf(cons, font_ASCII[0x53], cons->now_col, cons->now_row); 
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
    cons->command_num = 0;
    cons->now_col = 0;
    cons->now_row = 0;
    for(i = cons->vram_start; i < cons->vram_end; i++){
        cons->print_buf[j] = 0;
        j++;
    }
    console_setbuf(cons, font_ASCII[0x4e], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[0x54], cons->now_col, cons->now_row);
    console_setbuf(cons, font_ASCII[0x2d], cons->now_col, cons->now_row);   
    console_setbuf(cons, font_ASCII[0x4f], cons->now_col, cons->now_row); 
    console_setbuf(cons, font_ASCII[0x53], cons->now_col, cons->now_row); 
    console_setbuf(cons, prompt, cons->now_col, cons->now_row);

    consbuf_to_vram(cons);
    return;
}

//指定した範囲(行))のコンソール画面をリセットをする。
void console_subreset(CONSOLE *cons, int row){
    return;
}


//vimのmainプログラム
void vim_main(VIM *vim, CONSOLE *cons){
    //テキストエディタを立ち上げる
    BUFF *key_bf;
    key_bf = 0x00010010;
    unsigned char data;
    int flg;
    //コンソールのリセットを行う。
    console_reset(cons);
    //vim用の画面を立ち上げる。
    startup_vim(vim, cons);
    unsigned int count=0;
    //mainループ
    loop:while(1){
        //なぜかこのif文を入れないとバグが発生する。調査中
        if(key_bf->data_num >100){
        print_font_ascii(65, 30, 30);
        }
        if(key_bf->data_num>=1){
            //CLI命令ここで使う方がいいかも?
            data = pop_buff(key_bf);        //バッファからデータを取り出す。

            //STI命令で解除
            //Enterを押したとき
            if(data == 0x1c){
                vim_nextline(vim); 
                goto loop;      //while分の先頭へ
            }else if(data==0x1d){//コントロールキーを押した時、

                //PRESS+KEYをタイトルに書き込む
                vim_titlesetbuf(vim, font_ASCII['P'], 1, 44, 15);
                vim_titlesetbuf(vim, font_ASCII['R'], 1, 45, 15);
                vim_titlesetbuf(vim, font_ASCII['E'], 1, 46, 15);
                vim_titlesetbuf(vim, font_ASCII['S'], 1, 47, 15);
                vim_titlesetbuf(vim, font_ASCII['S'], 1, 48, 15);
                vim_titlesetbuf(vim, font_ASCII[' '], 1, 49, 15);
                vim_titlesetbuf(vim, font_ASCII['C'], 1, 50, 15);
                vim_titlesetbuf(vim, font_ASCII['R'], 1, 51, 15);
                vim_titlesetbuf(vim, font_ASCII['T'], 1, 52, 15);
                vim_titlesetbuf(vim, font_ASCII['L'], 1, 53, 15);
                vim_titlesetbuf(vim, font_ASCII['+'], 1, 54, 15);
                vim_titlesetbuf(vim, font_ASCII['S'], 1, 55, 15);
                vim_titlesetbuf(vim, font_ASCII['-'], 1, 56, 15);
                vim_titlesetbuf(vim, font_ASCII['K'], 1, 57, 15);
                vim_titlesetbuf(vim, font_ASCII['E'], 1, 58, 15);
                vim_titlesetbuf(vim, font_ASCII['Y'], 1, 59, 15);

                //vramに書き込む
                vimbuf_to_vram(vim);              
                for(;;){
                    data = pop_buff(key_bf);
                    if(data <= 127 && data>=0){
                        if(data==0x1f){//Sキーを押したら終了する。
                            //ファイルの保存をする。
                            //save_file(vim);
                            vim->vim_file[vim->vim_file_num] = 0x1a;
                            store_file(&vim->vim_file, &vim->filename);//こっちをやる予定。
                            console_reset(cons);
                            return;
                        }
                        
                        if(data != 0x1f){//違うの押したら、元に戻る。
                            goto loop;
                        }
                    }
                }
            }
            vim->vim_file[vim->vim_file_num]=key_table[data];
            vim->vim_file_num++;
            vim_setbuf(vim, font_ASCII[key_table[data]], vim->now_row, vim->now_col);
            vimbuf_to_vram(vim);
        }
    }
    //vimを終了するので、コンソールを立ち上げる。
    console_reset(cons);
    return; //vimコマンド終了する。

}

//vimの画面を立ち上げる。
void startup_vim(VIM *vim, CONSOLE *cons){
    int i, j;
    j = 0;

    //変数の初期化を行う。
    vim->filename[0]='H';
    vim->filename[1]='E';
    vim->filename[2]='L';
    vim->filename[3]='L';
    vim->filename[4]='O';
    vim->filename[5]=' ';
    vim->filename[6]=' ';
    vim->filename[7]=' ';
    vim->filename[8]='T';
    vim->filename[9]='S';
    vim->filename[10]='T';

    //char vim_buf[1024*768]; //vimで使用できるテキストのバッファ
    //char command[10];  //vimコマンド
    vim->vram_start=cons->vram_start;         //vramの開始番地
    vim->vram_end=cons->vram_end;           //vramの終了番地

    vim->now_row=2, vim->now_col=3;   //今、何行何列にいるのか?
    vim->max_row=47, vim->max_col=102;   //最大何行何列
    vim->text_row=0, vim->text_col=2; //テキスト領域の中での、今何行何列にいるのか?
    vim->text_max_row=1024, vim->text_max_col=768;//テキストエリアの最大何行何列
    vim->vim_file_num=0;
    //画面を初期化する。
    for(i = cons->vram_start; i < cons->vram_end; i++){
        vim->vim_buf[j] = 15;           //とりあえず画面を白く
        *((char*)i) = 15;
        j++;
    }

    //タイトルの背景色
    //iが、y座標
    //jが、x座標
    for(i = 0;i<32;i++){
        for(j=0; j < 1024; j++){
            vim->vim_buf[j+1024*i] = 0;
        }
    }
    
    for(i = 736;i<768;i++){
        for(j=0; j < 1024; j++){
            vim->vim_buf[j+1024*i] = 0;
        }
    }

    char *s;
    //char* sprintf_nt(char *s, int x)を使って埋め込む
    vim_titlesetbuf(vim, font_ASCII[0x56], 0, 38, 15);
    vim_titlesetbuf(vim, font_ASCII[0x49], 0, 39, 15);
    vim_titlesetbuf(vim, font_ASCII[0x4d], 0, 40, 15);
    
    vim_titlesetbuf(vim, font_ASCII[0x45], 0, 42, 15);
    vim_titlesetbuf(vim, font_ASCII[0x44], 0, 43, 15);
    vim_titlesetbuf(vim, font_ASCII[0x49], 0, 44, 15);
    vim_titlesetbuf(vim, font_ASCII[0x54], 0, 45, 15);

    for(i=2; i < 46; i++){
        sprintf_nt(s, i-2);
        vim_setbuf(vim, font_ASCII[s[1]], i, 0);
        vim_setbuf(vim, font_ASCII[s[0]], i, 1);
        vim_setbuf(vim, font_ASCII[0x3a], i, 2);
    }
    vim_titlesetbuf(vim, font_ASCII[0x53], 46, 45, 12);
    vim_titlesetbuf(vim, font_ASCII[0x41], 46, 46, 12);
    vim_titlesetbuf(vim, font_ASCII[0x56], 46, 47, 12);
    vim_titlesetbuf(vim, font_ASCII[0x45], 46, 48, 12);
    vim_titlesetbuf(vim, font_ASCII[0x3a], 46, 49, 12);
    vim_titlesetbuf(vim, font_ASCII[0x43], 46, 50, 12);
    vim_titlesetbuf(vim, font_ASCII[0x52], 46, 51, 12);
    vim_titlesetbuf(vim, font_ASCII[0x54], 46, 52, 12);
    vim_titlesetbuf(vim, font_ASCII[0x4c], 46, 53, 12);
    vim_titlesetbuf(vim, font_ASCII[0x2b], 46, 54, 12);
    vim_titlesetbuf(vim, font_ASCII[0x53], 46, 55, 12);

    //添字描画の時、加算されているため、リセットする。
    vim->now_col = 3;
    vim->now_row = 2;

    vimbuf_to_vram(vim);
    return;
}

//vimの新しい行に移動する。
void vim_nextline(VIM *vim){
    vim->now_col=3;
    vim->now_row++;
    //最大行まで来た時の処理は、考え中
    //consoleと同じようにするわけにもいかない。(書き込んだ文字列全て保存したい。)
    //動的にメモリ確保して、スクロール実装するかも?
}

//vimのバッファの内容をvramへ
void vimbuf_to_vram(VIM *vim){
    int i, j;
    j = 0;
    for(i = vim->vram_start; i < vim->vram_end; i++){
        *((char*)i) = vim->vim_buf[j++];
    }
}

//タイトル枠に、文字列を書き込む。タイトルに限り色を指定可能。
void vim_titlesetbuf(VIM *vim, char *font, int row, int col, int color){
    int x, y;
    x = 10*col;
    y = 16*row;
    int i;
    char data;

    for(i=0; i < 16; i++){
		data = font[i];

		if((data & 0x80) != 0){
			vim->vim_buf[x+1024*(i+y)] = color;
		}
		if((data & 0x40) != 0){
			vim->vim_buf[x+1+1024*(i+y)] = color;
		}
		if((data & 0x20) != 0){
			vim->vim_buf[x+2+1024*(i+y)] = color;
        }
		if((data & 0x10)  != 0){
			vim->vim_buf[x+3+1024*(i+y)] = color;
		}
		if((data & 0x08) != 0){
			vim->vim_buf[x+4+1024*(i+y)] = color;
		}
		if((data & 0x04) != 0){
			vim->vim_buf[x+5+1024*(i+y)] = color;
		}
		if((data & 0x02) != 0){
			vim->vim_buf[x+6+1024*(i+y)] = color;
		}
		if((data & 0x01) != 0){
			vim->vim_buf[x+7+1024*(i+y)] = color;
		}
	}

    return;
}

//vimのタイトルを入力する。
void vim_title(){
    return;
}

//vimのバッファに描画データを送る。
void vim_setbuf(VIM *vim, char *font, int row, int col){
    int x, y;
    x = 10*col;
    y = 16*row;
    int i;
    char data;

    for(i=0; i < 16; i++){
		data = font[i];

		if((data & 0x80) != 0){
			vim->vim_buf[x+1024*(i+y)] = 0;
		}
		if((data & 0x40) != 0){
			vim->vim_buf[x+1+1024*(i+y)] = 0;
		}
		if((data & 0x20) != 0){
			vim->vim_buf[x+2+1024*(i+y)] = 0;
        }
		if((data & 0x10)  != 0){
			vim->vim_buf[x+3+1024*(i+y)] = 0;
		}
		if((data & 0x08) != 0){
			vim->vim_buf[x+4+1024*(i+y)] = 0;
		}
		if((data & 0x04) != 0){
			vim->vim_buf[x+5+1024*(i+y)] = 0;
		}
		if((data & 0x02) != 0){
			vim->vim_buf[x+6+1024*(i+y)] = 0;
		}
		if((data & 0x01) != 0){
			vim->vim_buf[x+7+1024*(i+y)] = 0;
		}
	}

    vim->now_col++;
    //最後の列まできた
    if(vim->now_col == vim->max_col){
        vim->now_col = 3;
        vim->now_row++;        
        if(vim->now_row >= vim->max_row){ //最後の行まできたs
            vim->now_row = 2;
            //console_reset(cons);ここで強制セーブにする予定
        }
    }
 
    return;
}


void ls(FILE_ENTRY* entry, CONSOLE*cons){

    //下の数行は関数化する予定
    cons->now_row++;
    cons->now_col=0;
    //最終行まで来た時
    if(cons->now_row >= cons->max_row){
        console_reset(cons);
    }

	//カウンタ変数
	int i, j;
	int entry_num=3;
	int str_len;
	FILE_ENTRY *entry_now;
	
	//線形探索
	for(i=0; i < entry_num; i++){
		entry_now = (entry+i);	//今、注目しているエントリの番地をentry_nowに入れる
		//ファイルがあった。
		if(entry_now->filename[0] != 0){		
			char *str;
			str = &entry_now->filename;
			str_len = len(str);

			for(j=0; j < str_len; j++){
				if(*(str+j) == 32){	//スペースキーの場合、描画しない。
					continue;
				}else{
                    if(j==8){
                        console_setbuf(cons, font_ASCII[0x2e], cons->now_col, cons->now_row);
                        console_setbuf(cons, font_ASCII[*(str+j)], cons->now_col, cons->now_row);
                    }else{
                        console_setbuf(cons, font_ASCII[*(str+j)], cons->now_col, cons->now_row);
                    }
				}
			}
		}
	}
                                
    consbuf_to_vram(cons);
}

//他のプログラムが画面に映したいものがあれば、この関数を利用する。
void print_any(char *file, int file_size){

    return;
}