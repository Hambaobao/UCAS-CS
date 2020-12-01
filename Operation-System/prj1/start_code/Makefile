CC = mipsel-linux-gcc

all: image

bootblock: bootblock.s
	${CC} -G 0 -O2  -fno-pic -mno-abicalls -fno-builtin -nostdinc -mips3 -Ttext=0xffffffffa0800000 -N -o bootblock bootblock.s -nostdlib -e main -Wl,-m -Wl,elf32ltsmip -T ld.script

kernel: kernel.c
	${CC} -G 0 -O2  -fno-pic -mno-abicalls -fno-builtin -nostdinc -mips3 -Ttext=0xffffffffa0800200 -N -o kernel kernel.c -nostdlib -Wl,-m -Wl,elf32ltsmip -T ld.script		

createimage:
	gcc createimage.c -o createimage

image: bootblock kernel
	./createimage --extended bootblock kernel

clean:
	rm -rf bootblock image kernel *.o

floppy:
	sudo fdisk -l /dev/sdb
	sudo dd if=image of=/dev/sdb conv=notrunc
