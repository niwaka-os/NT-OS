//font_ASCII対応
char font_ASCII[128][16];
const char font_SPACE[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//32
const char font_0[16]={0x00,0x7f,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x7f,0x00,0x00};//48
const char font_1[16]={0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00};//49
const char font_2[16]={0x00,0x3c,0x42,0x41,0x41,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x7f,0x00,0x00};//50
const char font_3[16]={0x00,0x3c,0x42,0x01,0x01,0x01,0x01,0x3e,0x02,0x01,0x01,0x01,0x42,0x3c,0x00,0x00};//51
const char font_4[16]={0x00,0x10,0x20,0x48,0x88,0x88,0xff,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00};//52
const char font_5[16]={0x00,0x7f,0x40,0x40,0x40,0x40,0x7f,0x01,0x01,0x01,0x01,0x01,0x01,0x7e,0x00,0x00};//53
const char font_6[16]={0x00,0x1f,0x20,0x40,0x40,0x40,0x40,0x7f,0x41,0x41,0x41,0x41,0x41,0x7e,0x00,0x00};//54
const char font_7[16]={0x00,0x7f,0x41,0x41,0x41,0x41,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00};//56
const char font_8[16]={0x00,0x7f,0x41,0x41,0x41,0x41,0x7f,0x41,0x41,0x41,0x41,0x41,0x41,0x7f,0x00,0x00};//57
const char font_9[16]={0x00,0x7f,0x41,0x41,0x41,0x41,0x7f,0x01,0x01,0x01,0x01,0x01,0x01,0x7f,0x00,0x00};//58
const char font_A[16]={0x00,0x18,0x18,0x18,0x18,0x24,0x24,0x24,0x24,0x7e,0x42,0x42,0x42,0xe7,0x00,0x00};//65に用意予定
const char font_B[16]={0x00,0x7e,0x41,0x41,0x41,0x42,0x7e,0x41,0x41,0x41,0x41,0x41,0x42,0x7e,0x00,0x00};//66に用意予定
const char font_C[16]={0x00,0x0c,0x12,0x21,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x21,0x12,0x0c,0x00,0x00};//67に用意予定
const char font_D[16]={0x00,0x78,0x44,0x42,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x44,0x78,0x00,0x00};//68
const char font_E[16]={0x00,0x7f,0x40,0x40,0x40,0x40,0x40,0x7f,0x40,0x40,0x40,0x40,0x40,0x7f,0x00,0x00};//69
const char font_F[16]={0x00,0x7f,0x40,0x40,0x40,0x40,0x7f,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00};//70
const char font_G[16]={0x00,0x0c,0x12,0x21,0x40,0x40,0x40,0x4f,0x41,0x41,0x41,0x21,0x13,0x0d,0x00,0x00};//71
const char font_H[16]={0x00,0x41,0x41,0x41,0x41,0x41,0x41,0x7f,0x41,0x41,0x41,0x41,0x41,0x41,0x00,0x00};//72
const char font_I[16]={0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x00,0x00};//73
const char font_J[16]={0x00,0xff,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x44,0x44,0x38,0x00,0x00};//74
const char font_K[16]={0x00,0x40,0x41,0x42,0x44,0x48,0x50,0x60,0x60,0x50,0x48,0x44,0x42,0x41,0x00,0x00};//75
const char font_L[16]={0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7f,0x00,0x00};//76
const char font_M[16]={0x00,0x81,0xc3,0xa5,0xa5,0xa5,0x99,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x00,0x00};//77
const char font_N[16]={0x00,0x81,0xc1,0xa1,0xa1,0xa1,0x91,0x89,0x89,0x85,0x85,0x85,0x83,0x81,0x00,0x00};//78
const char font_O[16]={0x00,0x18,0x24,0x42,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x42,0x3c,0x00,0x00};//79
const char font_P[16]={0x00,0xf8,0x84,0x82,0x82,0x82,0x82,0xfc,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00};//80
//const char font_Q[16]={};//81
//const char font_R[16]={};//82
const char font_S[16]={0x00,0x7e,0x41,0x41,0x41,0x41,0x40,0x7c,0x03,0x01,0x41,0x41,0x42,0x7c,0x00,0x00};//83
const char font_Y[16]={0x00,0x41,0x22,0x14,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00};//89

//fontを初期化する。
void font_init(){
    int i;
    int j;
    for(i=0; i < 128; i++){
        for(j=0; j < 16; j++){
            font_ASCII[i][j] = 0x00;
        }
    }
    for(i=0; i < 16; i++){
        font_ASCII[32][i] = font_SPACE[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[48][i] = font_0[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[49][i] = font_1[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[50][i] = font_2[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[51][i] = font_3[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[52][i] = font_4[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[53][i] = font_5[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[54][i] = font_6[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[55][i] = font_7[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[56][i] = font_8[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[57][i] = font_9[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[65][i] = font_A[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[66][i] = font_B[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[67][i] = font_C[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[68][i] = font_D[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[69][i] = font_E[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[70][i] = font_F[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[71][i] = font_G[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[72][i] = font_H[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[73][i] = font_I[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[74][i] = font_J[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[75][i] = font_K[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[76][i] = font_L[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[77][i] = font_M[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[78][i] = font_N[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[79][i] = font_O[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[80][i] = font_P[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[83][i] = font_S[i];
    }
    for(i=0; i < 16; i++){
        font_ASCII[89][i] = font_Y[i];
    }
}
