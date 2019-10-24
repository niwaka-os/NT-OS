#define DEF_PORT_DMAC0_STATUS       0x08  
#define DEF_PORT_DMAC0_COMMAND      0x08  
#define DEF_PORT_DMAC0_REQUEST      0x09  
#define DEF_PORT_DMAC0_SINGLE_MASK  0x0A  
#define DEF_PORT_DMAC0_MODE         0x0B  
#define DEF_PORT_DMAC0_CLEAR_BP     0x0C 
#define DEF_PORT_DMAC0_TEMPORARY    0x0D  
#define DEF_PORT_DMAC0_MASTER_CLEAR 0x0D  
#define DEF_PORT_DMAC0_MASK_RESET   0x0E 
#define DEF_PORT_DMAC0_ALL_MASK     0x0F  


//DMA初期化する
void init_dma(short addr, short size){
    //maskする。
    out_8(DEF_PORT_DMAC0_SINGLE_MASK, 0x06);
    //DMAのリセットする。
    out_8(DEF_PORT_DMAC0_CLEAR_BP, 0xff);
    //転送先のアドレスを指定する。
    out_8(0x04, addr & 0x00ff);
    out_8(0x04, (addr & 0xff00) >> 8);
    out_8(DEF_PORT_DMAC0_CLEAR_BP, 0xff);
    //転送サイズを指定する。
    out_8(0x05, size & 0x00ff);
    out_8(0x05, (size & 0xff00) >> 8);
    //これ何?
    out_8(0x81, 0);
    //mask解除する。
    out_8(DEF_PORT_DMAC0_SINGLE_MASK, 0x02);

    return;
}

//読み込みモードにする。
void read_mode_dma(){
    out_8(DEF_PORT_DMAC0_SINGLE_MASK, 0x06);
    out_8(DEF_PORT_DMAC0_MODE, 0x56);
    out_8(DEF_PORT_DMAC0_SINGLE_MASK, 0x02);

    return;
}

//書き込みモードにする。
void write_mode_dma(){
    out_8(DEF_PORT_DMAC0_SINGLE_MASK, 0x06);
    out_8(DEF_PORT_DMAC0_MODE, 0x5a);
    out_8(DEF_PORT_DMAC0_SINGLE_MASK, 0x02);

    return;
}
