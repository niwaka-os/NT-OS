#include "NT-OS.h"
//FDCレジスタ
#define STATUS_REGISTER_A 0x3F0
#define STATUS_REGISTER_B 0x3F1
#define DIGITAL_OUTPUT_REGISTER 0x3F2
#define TAPE_DRIVE_REGISTER     0x3F3
#define MAIN_STATUS_REGISTER    0x3F4
#define DATARATE_SELECT_REGISTER 0x3F4
#define DATA_FIFO 0x3F5
#define DIGITAL_INPUT_REGISTER 0x3F7
#define CONFIGURATION_CONTROL_REGISTER 0x3F7

//コマンド
#define WRITE_DATA 0x05
#define READ_DATA  0x06

//asmfunc.asm
void out_8(int port, char value);
static char ReceivedIRQ6 = 0;  //1:割り込みきた 0:割り込み来てない。
static char motor = 0;         //1:モーターオン 0:モーターオフ

//DMA.c
void init_dma(short addr, short size);
void read_mode_dma();
void write_mode_dma();

void inthandler26(int *esp){

    out_8(PIC0_OCW2, 0x66);
    ReceivedIRQ6 = 1;
    return;

}

//フロッピーディスクの初期化する
void flpc_reset(){
    //コントローラのリセットする
    out_8(DIGITAL_OUTPUT_REGISTER, 0x00);
    //コントローラの有効化する
    out_8(DIGITAL_OUTPUT_REGISTER, 0x04);
    return;
}

//MSRの7bit目と6bit目をチェックする
void msr_check(){
    for(;;){
        if((in_8(MAIN_STATUS_REGISTER) & 0xc0) == 0x80){
            break;
        }
    }
}

//result_phaseのチェックする。
void msr_check_resultphase(){
    for(;;){
        if((in_8(MAIN_STATUS_REGISTER) & 0x50) == 0x50){
            break;
        }
    }
    char result;
    result = in_8(DATA_FIFO);
    return;
}

//IRQ発生まち
void flpyirq_wait(){
    while(ReceivedIRQ6 != 1){
        //何もしない。
    }
    ReceivedIRQ6 = 0;    //また0にする。
    return;
}

//コマンドを発行する。
void issue(int port, char command){
    //FDCの準備まち
    msr_check();
    out_8(port, command);
    return;
}

//DMAを有効化する。
void dma_valid(){
    msr_check();
    out_8(DATA_FIFO, 0x03);
    msr_check();
    out_8(DATA_FIFO, 0x00);
    msr_check();
    out_8(DATA_FIFO, 0x00);
    return;
}

//motorをオンにする。
void motor_on(){
    if(motor != 1){
        out_8(DIGITAL_OUTPUT_REGISTER, 0x1c);
        motor = 1;
    }
    return;
}


//readコマンドを発行する。
void issue_command_read(){

    short addr = 0x3000;
    short size = 1024;

    motor_on();
    init_dma(addr, size);
    dma_valid();
    read_mode_dma();

    //readコマンドを発行する。
    issue(DATA_FIFO, 0x06);
    //シリンダ番号0で、ドライブ番号0を読み込む。
    issue(DATA_FIFO, 0x00);
    //シリンダ番号0を発行する。
    issue(DATA_FIFO, 0x00);
    //ヘッド番号を発行する。
    issue(DATA_FIFO, 0x00);
    //セクタ番号を発行する。
    issue(DATA_FIFO, 0x01);
    //セクタサイズを発行する。
    issue(DATA_FIFO, 0x02);
    //トラック長を指定する。
    issue(DATA_FIFO, 0x12);
    //ギャップサイズを発行する。
    issue(DATA_FIFO, 0x1b);
    //データ長を発行する。
    issue(DATA_FIFO, 0xff);

    //IRQまち
    flpyirq_wait();

    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();

    return;
}

void issue_command_write(){

    short addr = 0x3000;
    short size = 1024;

    init_dma(addr, size);
    write_mode_dma();

    //WRITE_DATAを発行する。
    issue(DATA_FIFO, 0x05);
    //ヘッド番号0で、ドライブ番号0を読み込む
    issue(DATA_FIFO, 0x04);
    //シリンダ番号0を発行する。
    issue(DATA_FIFO, 0x00);
    //ヘッド番号1を発行する。
    issue(DATA_FIFO, 0x01);
    //セクタ番号2を発行する。
    issue(DATA_FIFO, 0x02);
    //セクターサイズを発行する。
    issue(DATA_FIFO, 0x02);
    //トラック長を発行する。
    issue(DATA_FIFO, 0x12);
    //ギャップサイズを発行する。
    issue(DATA_FIFO, 0x1b);
    //データ長を発行する。
    issue(DATA_FIFO, 0xff);

    //IRQ発生まち。
    flpyirq_wait();

    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();
    msr_check_resultphase();

    return;
}