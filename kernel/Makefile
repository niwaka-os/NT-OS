TOOLPATH =;ここはみなさんのパスを!!
NT-OSPATH=;ここはみなさんのパスを!!
MAKE     = make
NASM     = $(TOOLPATH)nasm
GCC      = $(TOOLPATH)i386-elf-gcc

ipl.bin : ipl.asm Makefile 
	  $(NASM) ipl.asm -o ipl.bin

protect_on.bin:protect_on.asm Makefile
	$(NASM) -o protect_on.bin protect_on.asm

asmfunc.o: asmfunc.asm Makefile
	$(NASM) -f elf32 -o asmfunc.o asmfunc.asm

main.o: main.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o main.o main.c

font.o: font.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o font.o font.c

floppy.o: floppy.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o floppy.o floppy.c

dma.o: dma.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o dma.o dma.c

keyboard.o: keyboard.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o keyboard.o keyboard.c

gdt-idt.o: gdt-idt.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o gdt-idt.o gdt-idt.c

pic.o : pic.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o pic.o pic.c

console.o : console.c Makefile
	$(GCC) -O -c -m32 -fno-pic -o console.o console.c

main.bin: main.o asmfunc.o console.o font.o floppy.o dma.o gdt-idt.o pic.o keyboard.o Makefile
	i386-elf-ld -nostdlib -nostartfiles -e NTMAIN -o main.bin -T os.ls main.o asmfunc.o font.o floppy.o gdt-idt.o dma.o pic.o console.o keyboard.o

os.sys: protect_on.bin main.bin Makefile
	cat protect_on.bin main.bin > os.sys

os.img  : ipl.bin  os.sys Makefile
	mformat -f 1440 -C -B ipl.bin -i os.img ::
	mcopy -i os.img os.sys ::
	mcopy -i os.img $(NT-OSPATH)fs/fs.c   ::

run:os.img Makefile
	$(MAKE) os.img
	 /usr/local/bin/qemu-system-i386 -fda os.img -boot a

formatFAT32:Makefile
	mformat -h 300 -t 30 -n 30 -F -C -B ipl.bin -i os.img ::

transfertoFAT32:
	mcopy -i os.img main.bin ::

fs.elf:
	$(GCC) -O -m32 -fno-pic -o fs.elf ../fs/fs.c
