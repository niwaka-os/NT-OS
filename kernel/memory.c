//ページング実装

//ページングで使用するリニアアドレスの構造体
typedef struct page{
    int offset_low;     //オフセットの低位
    int offset_mid;     //オフセットの中位
    int offset_high;    //オフセットの高位
    int table_low;      //テーブルの低位
    int table_mid;      //テーブルの中位
    short table_high;   //テーブルの高位
    int dir_low;        //ディレクトリの低位
    int dir_mid;        //ディレクトリの中位
    short dirhigh;      //ディレクトリの高位
}PAGE;

//参考はじめて読む486のp.308
//ページテーブルの要素PTEのデータ構造
typedef struct page_talbe_pte{
    char attr1;
    char attr2_pagenumber_low;
    short page_number_high;
}PAGE_TABLE_PTE;

//ページディレクトリの要素PTEのデータ構造
typedef struct page_directory_pte{
    char attr1;
    char attr2_pagenumber_low;
    short page_number_high;
}PAGE_DIRECTORY_PTE;

PAGE_TABLE_PTE *page_table;
PAGE_DIRECTORY_PTE *page_directory;     //ページディレクトリの先頭のアドレス

void setpage_directory(PAGE_DIRECTORY_PTE *dir){

    int i; //カウンタ変数
    PAGE_DIRECTORY_PTE *dir_now;//今示している要素

    //カウンタ変数
    //ページディレクトリの初期化を行う。
    for(i=0; i < 1024; i++){
        dir_now = (dir+i);  //dir_nowに今注目している要素を代入する。
        dir_now->attr1 = 0x00;
        dir_now->attr2_pagenumber_low = 0x00;
        dir_now->page_number_high = 0x00;
    }

    return;
}

//各ページテーブルの初期化をする。
void setpage_alltable(PAGE_TABLE_PTE *table){
    int i;//カウンタ変数
    PAGE_TABLE_PTE *table_now;

    //各ページテーブルの初期化を行う。
    for(i=0;i < 1024;i++){
        table_now = (table+i)
        table_now->attr1 = 0x00;
        table_now->attr2 = 0x00;
        table_now->page_number_high =0x00;
    }

    return;
}
//ページフォールトハンドラ
void pagefaulthandler(int virtual_addr){
    return;
}

//pageinをする。
void pagein(){
    return;
}

//pageoutをする。
void pageout(){
    return;
}

//PTEのセットをする。
void set_pte(){
    return;
}

//キャッシュをフラッシュする。
void flush_tlb(){
    return;
}

//論理ページと対応する物理ページを解放する。
void alloc_pysaddr(){
    return;
}

//特定の論理ページの物理ページとの対応を解除する。
void release_viraddr(){
    return;
}