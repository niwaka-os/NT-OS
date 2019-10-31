#include  "../kernel/NT-OS.h"
//read_sectors_lba();//LBA方式で複数セクタを読み出す。

//名前で指定してファイルを呼び出す。
//void read_file(char *filename);

//ファイルを保存する。
//void write_file(char *filename);
short cal_lbanumber_to_clust_number(short clust_number);
short search_clustnumber();
short cal_startclustnumber();
void write_to_disk(short first_clust_number, int clust_numbers, char *file);

typedef struct fat12{
    char *fat12_table;
    short fat12_region[3072];     //FAT12領域
}FAT12;   

typedef struct fat12_info{
    short clustnumber;
    FILE_ENTRY *entry;
}FAT12_INFO;

short fat[3072];
FILE_ENTRY *fat12_entry;          //fat12のディレクトリ領域型へのポインタ ,read_fat12dir_region()を呼び出すことで決定する。

//FAT12のFAT領域を読み出す。
void read_fat12region(){
    char *fat_region;
    fat_region = 0x600000;       //0x6011ffまで使用する。
    int i;
    int j=0;
    int lba, sectors;
    lba = 1;
    sectors = 11;
    read_sectors_lba(lba, sectors, fat_region);       //sectorの開始番号とセクタの数

    short a1, a2;       //a1は下1、a2は上1
    short b1, b2;       //b1は下2、b2は上2

    //FAT領域のデータを読み込む。
    for(i=0; i < 4608; i+=3){//FAT領域の4608byte分の半分
        a1 = (short)(*(fat_region+i));
        a2 = (short)(*(fat_region+i+1))<<8;
        b1 = (short)(*(fat_region+i+1))>>4;
        b1 = b1 & 0x0f;
        b2 = (short)(*(fat_region+i+2))<<4;

        fat[j]   = (a1|a2) & 0x0fff;               //2byte分のまず1目のクラスタ
        fat[j+1] = (b1|b2) & 0x0fff;               //2byte分の二つ目のクラスタ
        j+=2;   //3byteたす。
    }

    return;
}

//FAT領域で空いてる場所を探す。
FAT12_INFO* search_fat12_region(){
    short start_number;
    //最初のクラスタ番号を決定する。
    start_number = cal_startclustnumber();

    FILE_ENTRY *file_entry_now;

    int i;
    //ファイル名の先頭が0x00の場合、空き領域を示している。
    for(i=0; i < 224; i++){
        file_entry_now = (fat12_entry+i);
        //ファイル名の先頭が0か?
        if(file_entry_now->filename[0] == 0x00){
            FAT12_INFO *file_info;
            file_info = 0x602e00;
            file_info->entry = file_entry_now;
            file_info->clustnumber = start_number;
            return file_info;//空き領域発見した。
        }
    }    
    return 0;   //0番地を返したとき、空き領域が無かったことにする。
}

//最初のクラスタ番号を決定する。
short cal_startclustnumber(){
    short start_clustnumber;

    //空いているクラスタ番号を探す。
    start_clustnumber = search_clustnumber();

    return start_clustnumber;
}

//空いているクラスタ番号を探す。
short search_clustnumber(){

    int i;

    //0と1は書き換えてはいけないルールなので、除外
    for(i=2; i < 3072; i++){
        //クラスタ番号0番があった。
        if(fat[i]==0x0000){
            break;
        }
    }

    return (short)i;
}

//FAT12のディレクトリ領域を読み出す。
void read_fat12dir_region(){
    int lba, sectors;
    lba = 19;
    sectors = 1;
    char *buf;

    fat12_entry = 0x601200;
    buf = fat12_entry;

    read_sectors_lba(lba, sectors, buf);
    return;
}

//FAT12領域をコンソール画面に表示する。
void print_fat12(){
    return;
}

//ファイルのサイズを求める。
//テキストファイルの終端の文字コードを0x1aとする。
void cal_filesize(char *file){

    return;
}
//LBA方式からCHS方式
//cylinder = LBA/(トラック当たりセクタ数*総ヘッド数)
//head     = (LBA/(トラック当たりセクタ数))%ヘッド数
//sector   = LBA%トラック当たり+1

//ファイル保存
//保存したファイルの場所
void store_file(char *file, char *filename){
    FAT12_INFO *file_info;
    int clust_number;
    //EOFの添字を探す。
    int end_index;
    end_index = search_EOF(file);
    //FAT領域の空きとクラスタ番号の空きを探す。
    file_info = search_fat12_region();

    FILE_ENTRY *file_entry_now;
    file_entry_now = file_info->entry;
    
    int i;
    for(i=0; i < 11; i++){
        file_entry_now->filename[i] = filename[i];
    }
    file_entry_now->first_logical_cluster = file_info->clustnumber;

    //ファイルサイズを決定する。
    file_entry_now->file_size = end_index+1;
    //必要なクラスタ数を求める
    clust_number = cal_clustnumber(file_entry_now->file_size);
    alloc_clust_chain(file_entry_now->first_logical_cluster, clust_number);//開始クラスタ番号から繋いでいく。
    write_to_disk(file_entry_now->first_logical_cluster, clust_number, file);//ディスクに保存する。
    char *p;
    p = DMA_BUFF;
    for(i=0; i < 512; i++){
        *(p+i) = 0;
    }

    //read_lba(53);//確認用
    save_fat12region();//fat領域をフロッpーディスクに保存する。

    return;//書き込み終了する。
}

//ファイルに必要なクラスタの数を求める。motomeru.
int cal_clustnumber(int file_size){
    int quotient; //商
    int remainder;//余り

    quotient =  file_size / 512;//切り捨て(商)
    remainder = file_size % 512;//余り

    //余りが１以上のとき、quotientを+1する。
    if(remainder>=1){
        quotient += 1;
    }

    return quotient;
}

//fileのEOFが格納されている添字を探す。
//テキストの終端文字コードは、アプリケーション側が必ず書き込んでおかなければならい。
int search_EOF(char *file){
    //0x1aが終端文字コードとする。
    char *file_now;
    
    int i=0;
    for(;;){
        file_now = (file+i);
        //終端文字かチェックする。
        if(*(file_now)==0x1a){
            break;
        }
        i++;
    }
    return i;
}

//クラスタを割り当てる。
void alloc_clust_chain(short clust_firstnumber, int clust_number){
    short i, j;
    short prev;
    prev = clust_firstnumber;


    for(j=0; j < clust_number;j++){

        //空きクラスタ番号探す。
        for(i=2; i<3072; i++){
            if(fat[i]==0 && i!=clust_firstnumber && j!=(clust_number-1)){
                fat[prev] = i;
                break;
            }
            if(fat[i]==0 && j==clust_number-1){
                fat[prev] = i;
                fat[i] = 0x0FFF;//終了番号
                return;//終了
            }
        }
        prev = i;
    }
    return;
}

//ファイルをディスクへ保存する。
void write_to_disk(short first_clust_number, int clust_numbers, char *file){
    int lba_number;
    int i;
    short clust_number;
    clust_number = first_clust_number;


    for(i=0; i < clust_numbers; i++){
        //LBA方式においての、論理セクタ番号を取得する。(CHSと違い、開始は0から)
        lba_number = cal_lbanumber_to_clust_number(clust_number);
        //DMA用のバッファにデータを転送する。
        trans_buff(file, i);//これいらなくね?
        //WARITEコマンドを発行する。
        write_lba(lba_number, (file+i*512));
        //次のクラスタ番号を取得する。
        clust_number = fat[clust_number];
        //最後のクラスタ番号に来た時終了する。
        if(clust_number == 0x0FFF){
            break;//終了する。
        }
    }
    return;
}

//バッファにデータを送る。
void trans_buff(char *file, int count){
    char* buff;//write用のバッファ
    buff = DMA_BUFF;

    int i;

    file = file + 512*count;
    for(i=0; i < 512; i++){
        *(buff) = *(file+i);
    }
}
//クラスタ番号に対応するディスクの論理番号を求める。
short cal_lbanumber_to_clust_number(short clust_number){
    int disk_addr;
    short sector_number;
    disk_addr = clust_number*512 + 0x003e00;

    sector_number = disk_addr/512;

    return sector_number;
}

//保存するデータ構造 -> short fat[3072];
//RAMに保管したclusterの表fat配列を保存する。
void save_fat12region(){
    short a1;
    short a2;

    char low;
    char middle;
    char high;
    char *FAT_REGION;
    FAT_REGION = 0x700000;

    int i, j;//カウンタ変数
    j=0;
    //FAT用にfat配列を圧縮する。(FATフォーマットのルール)
    for(i=0; i < 3071; i+=2){
        a1 = fat[i];
        a2 = fat[i+1];

        low    = (char)((a1) & 0x00FF);
        middle = (char)((((a1>>8)&0x000F) | ((a2<<4)&0x00F0)) & 0x00FF);
        high   = (char)((a2>>4) & 0x00FF);
        *(FAT_REGION+j+0) = low;
        *(FAT_REGION+j+1) = middle;
        *(FAT_REGION+j+2) = high;
        j += 3;
    }

    j = 1;
    //trans_buff(char *file, int count)でデータをDMAバッファに転送する。
    for(i=0; i < 10;i++){
        trans_buff(FAT_REGION, i);
        write_lba(j, (FAT_REGION+i*512));
        j++;
    }
    read_lba(1);//テスト用
    return;
}

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

//catコマンドみたいなやつ
void cat_file(){
    return;
}
