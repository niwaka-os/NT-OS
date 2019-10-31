//main.c
#define VRAM_ADR 0xfd000000		//VRAMの開始番地
unsigned int len(char* s);

void init_pic();
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
#define DMA_BUFF        0x3000

typedef struct idtr{
    unsigned short limit;
    unsigned int   base;
}IDTR;

typedef struct gdtr{
    unsigned short limit;
    unsigned int   base;
}GDTR;

//8byte
typedef struct idt_gate{
    short offset_low;
    short selector;
    short attr;
    short offset_high;
}IDT_GATE;

typedef struct gdt_gate{
    short offset_low;
    short selector;
    short attr;
    short offset_high;
}GDT_GATE;

//font.c
extern char font_A[16];
extern char font_B[16];
extern char prompt[16];
extern char font_ASCII[128][16];

//console.c
typedef struct console{
    int x, y;               //今の座標
    int now_col, now_row;   //今、何行何列にいるのか
    int max_col, max_row;   //最大行と最大列
    char *font;             //プロンプト
    char print_buf[1024*768];         //コンソール画面の下書き
    int vram_start;           //VRAMの番地を用意する。
    int vram_end;           //実際は、vram_end-1が、最終番地
    int vram_high;          //画面の高さ
    int vram_side;          //画面の横
    char command[100];       //コマンド
    int command_num;        //commandに格納したデータの数
    int command_max_num;    //commandに格納したマックスのデータ数
}CONSOLE;

//keyboard.c
//待ち行列の構造
typedef struct buff{
    char fifo[30];
    int rear;
    int front;
    int data_num;   //今のデータの数
    int max_len;
}BUFF;



//FATエントリ7168(0x1c00)
//FAT12のディレクトリのエントリ構造体
//32byte
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

//floppy.c
void read_lba(int lba);
void write_lba(int lba, char *buf);

//lib.c
void sprintf_nt(char *s, int x);


//elf形式ヘッダ
typedef struct elf32_ehdr{
    unsigned char e_ident[16];
    
}ELF32_EHDR;


//fs.c
void read_fat12region();
void read_fat12dir_region();
void store_file(char *file, char *filename);
//他のファイルでも使うことにした。
extern short fat[3072];
extern FILE_ENTRY *fat12_entry;