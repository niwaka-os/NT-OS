#include  "../NT-OS.h"

//ファイルシステムはカーネル外
typedef struct FILE{
    unsinged char name[], ext[3], type;
    char reserve[10];
    unsigned short time, data, clustno;
    unsigned int size;  
}FILE_INFO;

//LBA方式からCHS方式
//cylinder = LBA/(トラック当たりセクタ数*総ヘッド数)
//head     = (LBA/(トラック当たりセクタ数))%ヘッド数
//sector   = LBA%トラック当たり+1
