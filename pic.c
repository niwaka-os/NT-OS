#include "NT-OS.h"

void in_8();  //void in_8(int port);
void out_8(); //void out_8(int port, char value);

void init_pic(void){
	out_8(PIC0_IMR,  0xff  ); 
	out_8(PIC1_IMR,  0xff  ); 
	out_8(PIC0_ICW1, 0x11  );
	out_8(PIC0_ICW2, 0x20  ); 
	out_8(PIC0_ICW3, 1 << 2); 
	out_8(PIC0_ICW4, 0x01  ); 

	out_8(PIC1_ICW1, 0x11  ); 
	out_8(PIC1_ICW2, 0x28  ); 
	out_8(PIC1_ICW3, 2     ); 
	out_8(PIC1_ICW4, 0x01  );

	out_8(PIC0_IMR, 0xbf);
	//out_8(PIC0_IMR,  0xbb  );
	out_8(PIC1_IMR,  0xff  ); 

	return;
}
