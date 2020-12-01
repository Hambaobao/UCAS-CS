/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include "irq.h"
#include "test.h"
#include "stdio.h"
#include "sched.h"
#include "screen.h"
#include "common.h"
#include "syscall.h"
#include "string.h"
#include "regs.h"
#include "mm.h"
#include "mac.h"
#include "fs.h"

extern stack_top;
extern kernel_stack_top;
extern user_stack_top;
extern exit_process_numb;

void init_page_table(void)
{
	// stop use in prj5
}

static void init_shell()
{
	pcb_valid[1] = 1;

	pcb[1].pid = process_id++;
	pcb[1].type = USER_PROCESS;
	pcb[1].status = TASK_READY;

	pcb[1].prev = NULL;	
	pcb[1].next = NULL;
	
	pcb[1].kernel_context.regs[31] = (uint32_t)&init_handle;
	pcb[1].user_context.regs[31] = (uint32_t)&test_shell;

	pcb[1].kernel_context.cp0_epc = (uint32_t)&test_shell;
	pcb[1].user_context.cp0_epc = (uint32_t)&test_shell;

	pcb[1].kernel_context.cp0_status = 0x10008003;
	pcb[1].user_context.cp0_status = 0x10008003;

	pcb[1].kernel_context.regs[29] = pcb[1].kernel_stack_top;
	pcb[1].user_context.regs[29] = pcb[1].user_stack_top;

	pcb[1].priority = 1;
	priority_queue_push(&pcb[1]);	
}

static void init_pcb()
{
	int i;
	for(i = 0;i < MAXPRIORITY;i++)
		queue_init(&(ready_queue[i]));
	
	queue_init(&block_queue);
	queue_init(&sleep_queue);
	queue_init(&recv_block_queue);


	for(i = 0;i < NUM_MAX_TASK;i++)
	{
		pcb_valid[i] = 0;
		pcb[i].kernel_stack_top = stack_top;
		stack_top -= STACK_SIZE;
		pcb[i].user_stack_top = stack_top;
		stack_top -= STACK_SIZE;
	}

	pcb_valid[0] = 1;
	current_running = &pcb[0];
	pcb[0].status = TASK_RUNNING;
	pcb[0].priority = 1;

	init_shell();
}

static void init_exception_handler()
{
	exception_handler[0] = (int (*) (void))&handle_int;
	int i;
	for(i = 1;i <NUM_EXCEPTION; i++)
	{
		exception_handler[i] = (int (*) (void))&handle_other;
	}
	exception_handler[2] = (int (*) (void))&handle_TLB;
	exception_handler[3] = (int (*) (void))&handle_TLB;
	exception_handler[8] = (int (*) (void))&handle_syscall;
}

static void init_exception()
{
	init_exception_handler();

	// 1. Get CP0_STATUS
	init_cp0_status = GET_CP0_STATUS();

	// 2. Disable all interrupt
	init_cp0_status |= 0x1000fc01;
	init_cp0_status ^= 0x1;
	SET_CP0_STATUS(init_cp0_status);
	init_cp0_status |= 0x1;	
	
	// 3. Copy the level 2 exception handling code to 0x80000180
	memcpy((void *)0x80000000, (void *)TLBexception_handler_begin, TLBexception_handler_end-TLBexception_handler_begin);
	memcpy((void *)0x80000180, (void *)exception_handler_entry, exception_handler_end-exception_handler_begin);

	// 4. reset CP0_COMPARE & CP0_COUNT register
	SET_CP0_COUNT(0);
	SET_CP0_COMPARE(0x300000);
}

static void init_syscall(void)
{
	// init system call table.
	int i;
	for(i = 0;i < NUM_SYSCALLS;i++)
	{
		syscall[i] = (int (*) (void))&do_other;
	}
	syscall[SYSCALL_SLEEP              ] = (int (*)()) & do_sleep;
	syscall[SYSCALL_GETPID             ] = (int (*)()) & do_getpid;
	syscall[SYSCALL_BLOCK              ] = (int (*)()) & do_block;
	syscall[SYSCALL_UNBLOCK_ONE        ] = (int (*)()) & do_unblock_one;
	syscall[SYSCALL_UNBLOCK_ALL        ] = (int (*)()) & do_unblock_all;
	syscall[SYSCALL_WRITE              ] = (int (*)()) & screen_write;
	syscall[SYSCALL_CURSOR             ] = (int (*)()) & screen_move_cursor;
	syscall[SYSCALL_REFLUSH            ] = (int (*)()) & screen_reflush;
	syscall[SYSCALL_PS                 ] = (int (*)()) & do_ps;
	syscall[SYSCALL_CLEAR              ] = (int (*)()) & do_clear;
	syscall[SYSCALL_SPAWN              ] = (int (*)()) & do_spawn;
	syscall[SYSCALL_KILL               ] = (int (*)()) & do_kill;
	syscall[SYSCALL_EXIT               ] = (int (*)()) & do_exit;
	syscall[SYSCALL_WAIT               ] = (int (*)()) & do_wait;
	syscall[SYSCALL_MUTEX_LOCK_INIT    ] = (int (*)()) & do_mutex_lock_init;
	syscall[SYSCALL_MUTEX_LOCK_ACQUIRE ] = (int (*)()) & do_mutex_lock_acquire;
	syscall[SYSCALL_MUTEX_LOCK_RELEASE ] = (int (*)()) & do_mutex_lock_release;
	syscall[SYSCALL_CLEAR_ALL          ] = (int (*)()) & do_clear_all;
	syscall[SYSCALL_BARRIER_INIT       ] = (int (*)()) & do_barrier_init;
	syscall[SYSCALL_BARRIER_WAIT       ] = (int (*)()) & do_barrier_wait; 
	syscall[SYSCALL_SEMAPHORE_INIT     ] = (int (*)()) & do_semaphore_init;
	syscall[SYSCALL_SEMAPHORE_UP       ] = (int (*)()) & do_semaphore_up;
	syscall[SYSCALL_SEMAPHORE_DOWN     ] = (int (*)()) & do_semaphore_down;
	syscall[SYSCALL_CONDITION_INIT     ] = (int (*)()) & do_condition_init;
	syscall[SYSCALL_CONDITION_WAIT     ] = (int (*)()) & do_condition_wait;
	syscall[SYSCALL_CONDITION_SIGNAL   ] = (int (*)()) & do_condition_signal;
	syscall[SYSCALL_CONDITION_BROADCAST] = (int (*)()) & do_condition_broadcast;
	syscall[SYSCALL_INIT_MAC           ] = (int (*)()) & do_init_mac;
	syscall[SYSCALL_NET_SEND           ] = (int (*)()) & do_net_send;
	syscall[SYSCALL_NET_RECV           ] = (int (*)()) & do_net_recv;
	syscall[SYSCALL_WAIT_RECV		   ] = (int (*)()) & do_wait_recv_package;

	syscall[SYSCALL_MKFS		       ] = (int (*)()) & mkfs;
	syscall[SYSCALL_MKDIR			   ] = (int (*)()) & mkdir;
	syscall[SYSCALL_RMDIR			   ] = (int (*)()) & rmdir;
	syscall[SYSCALL_READDIR	           ] = (int (*)()) & read_dir;
	syscall[SYSCALL_FSINFO	           ] = (int (*)()) & fs_info;
	syscall[SYSCALL_ENTERFS	           ] = (int (*)()) & enter_fs;

	syscall[SYSCALL_FTOUCH		       ] = (int (*)()) & mknod;
	syscall[SYSCALL_FCAT			   ] = (int (*)()) & fcat;
	syscall[SYSCALL_FOPEN			   ] = (int (*)()) & fopen;
	syscall[SYSCALL_FCLOSE	           ] = (int (*)()) & fclose;
	syscall[SYSCALL_FREAD	           ] = (int (*)()) & fread;
	syscall[SYSCALL_FWRITE	           ] = (int (*)()) & fwrite;
	syscall[SYSCALL_FFIND			   ] = (int (*)()) & ffind;
	syscall[SYSCALL_FRENAME		       ] = (int (*)()) & frename;
	syscall[SYSCALL_FLN				   ] = (int (*)()) & flink;
	syscall[SYSCALL_FSEEK			   ] = (int (*)()) & fseek;
	syscall[SYSCALL_FREMOVE			   ] = (int (*)()) & fremove;

	syscall[SYSCALL_INFO			   ] = (int (*)()) & do_print_info;
}

static void init_fs_addr(void)
{
	FS_MEM_START = ((uint32_t)(&FILE_SYSTEM + 512)) & 0xfffffe00;

	Current_Direction   = (C_dentry_t *)	(FS_MEM_START);
	Direction_buff      = (C_dentry_t *)	(FS_MEM_START + 1 * 4096);
	inode_map_buff      = (uint8_t *)		(FS_MEM_START + 2 * 4096);
	File_Buff           = (uint8_t *)		(FS_MEM_START + 3 * 4096);      
	Current_Inode       = (inode_t *)		(FS_MEM_START + 4 * 4096);
	inode_buff          = (inode_t *)		(FS_MEM_START + 4 * 4096 + 1 * 512);
	superblock          = (superblock_t*)	(FS_MEM_START + 4 * 4096 + 2 * 512);
	inode               = (inode_t *)		(FS_MEM_START + 4 * 4096 + 3 * 512);
	fd_buff             = (fd_t *)			(FS_MEM_START + 4 * 4096 + 4 * 512);
}

static void init_fs(void)
{
	init_fs_addr();
    // TODO Read Superblock From SD to Memory
    sdread((char*)(superblock), FS_START_ADDR, sizeof(superblock_t));

    if(superblock->magic_number == FS_MAGIC_NUMBER)
	{
		get_root_direction();		
    	printk("> [INIT] File System has existed!          \n");
        return;
	}

    printk("> [INIT] Start initialize File System!              \n");

    printk("> [INIT] Initializing SuperBlock...                 \n");
    init_superblock();          // 1. Init SuperBlock

    printk("> [INIT] Initializing Block_Map...                  \n");	
    init_blockmap();            // 2. Init Block Map

    printk("> [INIT] Initializing Inode_Map...                  \n");	
    init_inodemap();            // 3. Init Inode Map

    printk("> [INIT] Initializing Inode...                      \n");	
    init_inode();               // 4. Init Inode

    printk("> [INIT] Initializing Root Direction	...         \n");	
    init_root_direction();   // 5. Current Direction	
}

// jump from bootloader.
// The beginning of everything >_< ~~~~~~~~~~~~~~
void __attribute__((section(".entry_function"))) _start(void)
{
	// Close the cache, no longer refresh the cache 
	// when making the exception vector entry copy
	asm_start();

	// init interrupt (^_^)
	init_exception();
	printk("> [INIT] Interrupt processing initialization succeeded.\n");

	// init system call table (0_0)
	init_syscall();
	printk("> [INIT] System call initialized successfully.\n");

	// init page table
//	init_page_table();

	// init file system
	init_fs();
	printk("> [INIT] File System initialized successfully.\n");
	
	// init Process Control Block (-_-!)
	init_pcb();
	printk("> [INIT] PCB initialization succeeded.\n");

	// init screen (QAQ)
	init_screen();
	printk("> [INIT] SCREEN initialization succeeded.\n");


	// TODO Enable interrupt
	SET_CP0_STATUS(init_cp0_status);

//	int i = 0;
	while (1)
	{
		// (QAQQQQQQQQQQQ)
		// If you do non-preemptive scheduling, you need to use it to surrender control
		// do_scheduler();
//		 printk("> [WHILE] %d\n",i);
//		 i++;
	};
	
	return;
}
