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

extern stack_top;
extern exit_process_numb;

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

/*
	int i;
	for(i = 0;i < MAXPRIORITY;i++)
		queue_init(&(ready_queue[i]));
	
	queue_init(&block_queue);
	queue_init(&sleep_queue);

	init_pcb_part1();
	init_pcb_part2();

	int pcb_amount = num_sched1_tasks - 1 + num_lock_tasks + num_timer_tasks + num_sched2_tasks;
	// pcb_amount = 10;
	// MAXPRIORITY = 5;
	pcb[1].priority = 1;
	pcb[2].priority = 2;
	pcb[3].priority = 3;
	pcb[4].priority = 4;
	pcb[5].priority = 5;
	pcb[6].priority = 1;
	pcb[7].priority = 2;
	pcb[8].priority = 3;
	pcb[9].priority = 4;
//	pcb[10].priority = 5;

	for(i = 1;i <= pcb_amount;i++)
		priority_queue_push(&pcb[i]);

*/

}

static void init_exception_handler()
{
//	exception_handler[0] = (uint32_t)&handle_int;
	exception_handler[0] = (int (*) (void))&handle_int;
	int i;
	for(i = 1;i <NUM_EXCEPTION; i++)
	{
		exception_handler[i] = (int (*) (void))&handle_other;
	}
	exception_handler[8] = (int (*) (void))&handle_syscall;
}

static void init_exception()
{
	init_exception_handler();

	// 1. Get CP0_STATUS
	init_cp0_status = GET_CP0_STATUS();

	// 2. Disable all interrupt
	init_cp0_status |= 0x10008001;
	init_cp0_status ^= 0x1;
	SET_CP0_STATUS(init_cp0_status);
	init_cp0_status |= 0x1;	
	
	// 3. Copy the level 2 exception handling code to 0x80000180
	bzero((void *)BEV0_EBASE, BEV0_OFFSET);
	memcpy((void *)0x80000180, (void *)exception_handler_entry, exception_handler_end-exception_handler_begin);
	bzero((void *)BEV1_EBASE, BEV1_OFFSET);
	memcpy((void *)0xbfc00380, (void *)exception_handler_entry, exception_handler_end-exception_handler_begin);
	
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
