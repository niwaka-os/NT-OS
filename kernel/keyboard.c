#include "NT-OS.h"
//keyboardに関するデバイスドライバ
#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47

//とりあえず、デバッグ用に利用する。
void print_font_ascii(char ascii_code, int x, int y);
void console_setbuf(CONSOLE *cons, char *font, int col, int row);
void consbuf_to_vram(CONSOLE *cons);
//これいらないかも?
static char ReceivedIRQ1 = 0;

//待ち行列の構造
typedef struct buff{
    char fifo[30];
    int rear;
    int front;
    int data_num;   //今のデータの数
    int max_len;
}BUFF;

BUFF *key_buf;

//keycode変換用
char key_table[52]={0x00, 0x00, '1', '2', '3', '4', '5', '6', 0x37, 0x38, 0x39, 0x30,0x00, 0x00, 0x00, 0x00, 0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x5b,0x4f, 0x50, 0x40, 0x7b, 0x1c, 0x1d, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x3b, 0x3a, 0x00, 0x2a, 0x7d, 0x5a, 0x58, 0x43, 0x56,0x42, 0x4e, 0x4d};

void init_buf(){
    key_buf = 0x00010010;
    key_buf->rear     = 0;   //次に格納するデータの場所
    key_buf->front    = 0;  //次に取り出すデータの場所
    key_buf->data_num = 0;    //今のデータの数
    key_buf->max_len  =30;
}

//fifoにデータを入れる
void push_buff(BUFF *bf, char value){

    if(bf->data_num < bf->max_len){
        if(bf->rear < bf->max_len){
            bf->fifo[bf->rear++] = value;
            bf->data_num++;
        }else{
            bf->rear = 0;
            bf->fifo[bf->rear++] = value;
            bf->data_num++;
        }
    }

    return;
}

//fifoからデータを取り出す
char pop_buff(BUFF *bf){

    if(bf->data_num >= 1){
        bf->data_num--;
        if(bf->front>=bf->max_len){
            bf->front = 0;
            return bf->fifo[bf->front++];
        }else{
            return bf->fifo[bf->front++];
        }
    }

    //データを取り出せなかった。
    return -1;
}

static int i;
static i = 0;
char* d;

void inthandler21(){
    out_8(PIC0_OCW2, 0x61);
    ReceivedIRQ1=1;
    unsigned char data;
    d = 0x20000;
    data = in_8(PORT_KEYDAT);

    CONSOLE *cons;
    cons = 0x100000;

    //break時ならば、描画しない。
    //if(data & 0x80 == 0x80){
        //push_buff(key_buf, data);
        //data = pop_buff(key_buf);      //ボタンを話した時のコードは捨てる。
      //  return;
    //}

    if(data <= 127){

        *((char*)d+i) = data;

        push_buff(key_buf, data);
        //print_font_ascii(key_table[pop_buff(key_buf)], 0+10*i, 0);
        console_setbuf(cons, font_ASCII[key_table[pop_buff(key_buf)]], cons->now_col, cons->now_row);
        consbuf_to_vram(cons);
        //print_font_ascii(key_table[data], 0+10*i, 0);
        
        i++;
    }
    return;
}

//キーボードコントローラからの送信待機
void wait_kbc(){
    for(;;){
        if((in_8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY)==0){
            break;
        }
    }
    return;
}

void init_kbc(){
    wait_kbc();
    out_8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_kbc();
    out_8(PORT_KEYDAT, KBC_MODE);
    return;
}