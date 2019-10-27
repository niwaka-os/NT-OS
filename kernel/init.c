//あらゆる初期化関数をここで取り扱う。
//keyboard.c
void init_buf();

//pic.
void init_pic();

//gdt-idt.c
void idt_init();

//font.c
void font_init();

//あらゆる初期化をここで実行する。
void all_init(){
    return;
}