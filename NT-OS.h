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

/***
typedef struct idt_gate{
    short offset_high;
    short attr;
    short selector;
    short offset_low;
}IDT_GATE;
***/

//8byte
typedef struct idt_gate{
    short offset_low;
    short selector;
    short attr;
    short offset_high;
}IDT_GATE;

//font.c
extern char font_A[16];
extern char font_B[16];
extern char prompt[16];
extern char font_ASCII[128][16];