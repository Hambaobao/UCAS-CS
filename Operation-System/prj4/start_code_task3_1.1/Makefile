CC = mipsel-linux-gcc

all: clean createimage image asm # floppy

SRC_BOOT 	= ./arch/mips/boot/bootblock.S

SRC_ARCH	= ./arch/mips/kernel/entry.S ./arch/mips/kernel/syscall.S ./arch/mips/pmon/common.c 
SRC_DRIVER	= ./drivers/screen.c
SRC_INIT 	= ./init/main.c
SRC_INT		= ./kernel/irq/irq.c
SRC_LOCK	= ./kernel/locking/lock.c ./kernel/locking/barrier.c ./kernel/locking/cond.c ./kernel/locking/sem.c
SRC_MM          = ./kernel/mm/memory.c
SRC_SCHED	= ./kernel/sched/sched.c ./kernel/sched/queue.c ./kernel/sched/time.c
SRC_SYSCALL	= ./kernel/syscall/syscall.c
SRC_LIBS	= ./libs/string.c ./libs/printk.c

SRC_TEST	= ./test/test_shell.c
SRC_TEST4	= ./test/test_project4/test4.h ./test/test_project4/process1.c ./test/test_project4/process2.c

SRC_IMAGE	= ./tools/createimage.c

bootblock: $(SRC_BOOT)
	${CC} -g -G 0 -O2 -fno-pic -mno-abicalls -fno-builtin -nostdinc -mips3 -Ttext=0xffffffffa0800000 -N -o bootblock $(SRC_BOOT) -nostdlib -e main -Wl,-m -Wl,elf32ltsmip -T ld.script

main : $(SRC_ARCH) $(SRC_DRIVER) $(SRC_INIT) $(SRC_INT) $(SRC_LOCK) $(SRC_MM) $(SRC_SCHED) $(SRC_SYSCALL) $(SRC_LIBS) $(SRC_TEST) $(SRC_TEST4)
	${CC} -g -G 0 -O0 -Iinclude -Ilibs -Iarch/mips/include -Idrivers -Iinclude/os -Iinclude/sys -Itest -Itest/test_project4 \
	-fno-pic -mno-abicalls -fno-builtin -nostdinc -mips3 -Ttext=0xffffffffa0800200 -N -o main \
	$(SRC_ARCH) $(SRC_DRIVER) $(SRC_INIT) $(SRC_INT) $(SRC_LOCK) $(SRC_MM) $(SRC_SCHED) $(SRC_SYSCALL) $(SRC_PROC) $(SRC_LIBS) $(SRC_TEST) $(SRC_TEST4) -L. -lepmon -nostdlib -Wl,-m -Wl,elf32ltsmip -T ld.script		

createimage: $(SRC_IMAGE)
	gcc $(SRC_IMAGE) -g -o createimage

image: bootblock main
	./createimage --extended bootblock main

clean:
	rm -rf bootblock image createimage main *.o

floppy:
	sudo fdisk -l /dev/sdb
	sudo dd if=image of=/dev/sdb conv=notrunc

asm:
	mipsel-linux-objdump -d main > kernel.txt
