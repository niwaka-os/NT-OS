#include "NT-OS.h"
//asmfunc.asm
void lidt(short limit, int base);
//void lidt();
void asm_inthandler26();
void asm_inthandler21();

//gdtを初期化する。
void gdt_init(){
    return;
}

//gdtrにロードする。
void gdtr_load(){
    return;
}

//gdtのディスクリプたを作成する。
void create_gdt(){
    return;
}

void idtr_load(struct idtr* idt_register){
    idt_register->limit = 0x7ff;   //256*8=0x800を1だけ引いた値
    idt_register->base  = 0x00000000;
    //IDTRにidt_registerの値をロードする。
    lidt(idt_register->limit, idt_register->base);
}

//IDTに登録するディスクリプタを作成する。
void create_idt(struct idtr* idt_register, char vector_number, int offset, short attr, short selector){
    IDT_GATE desc;

    desc.offset_high = (short)(offset & 0xFFFF0000) >> 16;
    desc.attr        = (short)attr;
    desc.selector    = (short)selector;
    desc.offset_low  = (short)(offset & 0x0000FFFF);

    //指定したvector numberの要素に、descriptorを書き込む。
    IDT_GATE *pointer;
    pointer = idt_register->base;   //IDTのベースアドレスをIDTRに代入

    *(pointer+vector_number) = desc;

    return;
}   

//idt初期化する。
void idt_init(struct idtr* idt_register){
    //IDTRに、値をロードする。
    idtr_load(idt_register);

    int i;
    IDT_GATE desc;
    IDT_GATE *base_pointer;             //IDTのbaseポインター
    base_pointer = idt_register->base;

    //IDTを初期化する。
    for(i = 0; i < 256; i++){
        desc.offset_high = 0x0000;
        desc.attr        = 0x0000;
        desc.selector    = 0x0000;
        desc.offset_low  = 0x0000;
        *(base_pointer + i) = desc;
    }

    //floppyに関するdescriptorを作成する。
    create_idt(idt_register, 0x26, (int)asm_inthandler26, 0x8e00, 2*8);
    //keyboardに関するdescriptorを作成する。
    create_idt(idt_register, 0x21, (int)asm_inthandler21, 0x8e00, 2*8);
    return;
}