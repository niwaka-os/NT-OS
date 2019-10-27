//main.c
#define VRAM_ADR 0xfd000000		//VRAMの開始番地

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
}CONSOLE;


