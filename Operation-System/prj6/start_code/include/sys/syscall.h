/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                       System call related processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#ifndef INCLUDE_SYSCALL_H_
#define INCLUDE_SYSCALL_H_

#include "type.h"
#include "sync.h"
#include "queue.h"
#include "sched.h"

#define IGNORE                      0
#define NUM_SYSCALLS                64
#define NUM_EXCEPTION               32

/* define */
#define SYSCALL_SLEEP               1
#define SYSCALL_GETPID              2

#define SYSCALL_BLOCK               3
#define SYSCALL_UNBLOCK_ONE         4
#define SYSCALL_UNBLOCK_ALL         5

#define SYSCALL_WRITE               6
#define SYSCALL_READ                7
#define SYSCALL_CURSOR              8
#define SYSCALL_REFLUSH             9

#define SYSCALL_PS                  10
#define SYSCALL_CLEAR               11

#define SYSCALL_SPAWN               12
#define SYSCALL_KILL                13
#define SYSCALL_EXIT                14
#define SYSCALL_WAIT                15

#define SYSCALL_MUTEX_LOCK_INIT     16
#define SYSCALL_MUTEX_LOCK_ACQUIRE  17
#define SYSCALL_MUTEX_LOCK_RELEASE  18

#define SYSCALL_CLEAR_ALL           19

#define SYSCALL_BARRIER_INIT        20
#define SYSCALL_BARRIER_WAIT        21
#define SYSCALL_SEMAPHORE_INIT      22
#define SYSCALL_SEMAPHORE_UP        23
#define SYSCALL_SEMAPHORE_DOWN      24
#define SYSCALL_CONDITION_INIT      25
#define SYSCALL_CONDITION_WAIT      26
#define SYSCALL_CONDITION_SIGNAL    27
#define SYSCALL_CONDITION_BROADCAST 28

#define SYSCALL_INIT_MAC    29
#define SYSCALL_NET_SEND    30
#define SYSCALL_NET_RECV    31
#define SYSCALL_WAIT_RECV   32   

// Direction Operations
#define SYSCALL_MKFS        33
#define SYSCALL_MKDIR       34
#define SYSCALL_RMDIR       35
#define SYSCALL_READDIR     36
#define SYSCALL_FSINFO      37
#define SYSCALL_ENTERFS     38
// File System Operations
#define SYSCALL_FTOUCH      39
#define SYSCALL_FCAT        40
#define SYSCALL_FOPEN       41
#define SYSCALL_FCLOSE      42
#define SYSCALL_FREAD       43
#define SYSCALL_FWRITE      44
#define SYSCALL_FFIND       45
#define SYSCALL_FRENAME     46
#define SYSCALL_FLN         47
#define SYSCALL_FSEEK       48
#define SYSCALL_FREMOVE     49

#define SYSCALL_INFO        63      // Debug

/* syscall function pointer */
int (*syscall[NUM_SYSCALLS])();

/* exception function pointer*/
int (*exception_handler[NUM_EXCEPTION])();

void system_call_helper(int, int, int, int);
extern int invoke_syscall(int, int, int, int);

void sys_sleep(uint32_t);
pid_t sys_getpid();

void sys_block(queue_t *);
void sys_unblock_one(queue_t *);
void sys_unblock_all(queue_t *);

void sys_write(char *);
void sys_move_cursor(int, int);
void sys_reflush();

void sys_ps();
void sys_clear();

void sys_spawn(task_info_t *task);
void sys_kill(pid_t pid);
void sys_exit();
void sys_waitpid(pid_t pid);

void mutex_lock_init(mutex_lock_t *);
void mutex_lock_acquire(mutex_lock_t *);
void mutex_lock_release(mutex_lock_t *);

void barrier_init(barrier_t *barrier,int goal);
void barrier_wait(barrier_t *barrier);

void semaphore_init(semaphore_t *s, int val);
void semaphore_up(semaphore_t *s);
void semaphore_down(semaphore_t *s);

void condition_init(condition_t *condition);
void condition_wait(mutex_lock_t *lock, condition_t *condition);
void condition_signal(condition_t *condition);
void condition_broadcast(condition_t *condition);

void sys_net_send(uint32_t td,uint32_t td_phy);
uint32_t sys_net_recv(uint32_t rd,uint32_t rd_phy,uint32_t daddr);
void sys_init_mac(void);
void sys_wait_recv_package(void);

// Direction Operations
int sys_mkfs(void);
int sys_mkdir(char *dir_name);
int sys_rmdir(char *dir_name);
int sys_read_dir(char *path);
int sys_fs_info(void);
int sys_enter_fs(char *path);

// File System Operations
int sys_ftouch(char *fname);
int sys_fcat(char *fname);
int sys_fopen(char *fname, int access);
int sys_fclose(int fd);
int sys_fread(int fd, char *buff, int size);
int sys_fwrite(int fd, char *buff, int size);
int sys_ffind(char *path, char *fname);
int sys_frename(char *old_fname, char *new_fname);
int sys_fln(char *target, char *linkname, char *para);
int sys_fseek(int fd, uint32_t offset);
int sys_fremove(char *fname);

int sys_info(void);

#endif