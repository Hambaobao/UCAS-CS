#include "lock.h"
#include "sched.h"
#include "common.h"
#include "screen.h"
#include "syscall.h"

void system_call_helper(int fn, int arg1, int arg2, int arg3)
{
    // syscall[fn](arg1, arg2, arg3)
    current_running->user_context.cp0_epc = current_running->user_context.cp0_epc + 4;
    int ret = syscall[fn](arg1, arg2, arg3);
    current_running->user_context.regs[2] = ret;
}

void sys_sleep(uint32_t time)
{
    invoke_syscall(SYSCALL_SLEEP, time, IGNORE, IGNORE);
}

pid_t sys_getpid()
{
    invoke_syscall(SYSCALL_GETPID, IGNORE, IGNORE, IGNORE);
}

void sys_block(queue_t *queue)
{
    invoke_syscall(SYSCALL_BLOCK, (int)queue, IGNORE, IGNORE);
}

void sys_unblock_one(queue_t *queue)
{
    invoke_syscall(SYSCALL_UNBLOCK_ONE, (int)queue, IGNORE, IGNORE);
}

void sys_unblock_all(queue_t *queue)
{
    invoke_syscall(SYSCALL_UNBLOCK_ALL, (int)queue, IGNORE, IGNORE);
}

void sys_write(char *buff)
{
    invoke_syscall(SYSCALL_WRITE, (int)buff, IGNORE, IGNORE);
}

void sys_reflush()
{
    invoke_syscall(SYSCALL_REFLUSH, IGNORE, IGNORE, IGNORE);
}

void sys_ps()
{
    invoke_syscall(SYSCALL_PS,IGNORE,IGNORE,IGNORE);
}

void sys_clear()
{
    invoke_syscall(SYSCALL_CLEAR,IGNORE,IGNORE,IGNORE);
}

void sys_spawn(task_info_t *task)
{
    invoke_syscall(SYSCALL_SPAWN,(int)task,IGNORE,IGNORE);
}

void sys_kill(pid_t pid)
{
    invoke_syscall(SYSCALL_KILL,(int)pid,IGNORE,IGNORE);
}

void sys_exit()
{
    invoke_syscall(SYSCALL_EXIT,IGNORE,IGNORE,IGNORE);
}

void sys_waitpid(pid_t pid)
{
    invoke_syscall(SYSCALL_WAIT,(int)pid,IGNORE,IGNORE);    
}

void sys_clear_all()
{
    invoke_syscall(SYSCALL_CLEAR_ALL,IGNORE,IGNORE,IGNORE);
}

void barrier_init(barrier_t *barrier,int goal)
{   
    invoke_syscall(SYSCALL_BARRIER_INIT,(int)barrier,goal,IGNORE);
}

void barrier_wait(barrier_t *barrier)
{
    invoke_syscall(SYSCALL_BARRIER_WAIT, (int)barrier, IGNORE, IGNORE);
}

void semaphore_init(semaphore_t *s, int val)
{
    invoke_syscall(SYSCALL_SEMAPHORE_INIT, (int)s, val, IGNORE);
}

void semaphore_up(semaphore_t *s)
{
    invoke_syscall(SYSCALL_SEMAPHORE_UP, (int)s, IGNORE, IGNORE);
}

void semaphore_down(semaphore_t *s)
{
    invoke_syscall(SYSCALL_SEMAPHORE_DOWN, (int)s, IGNORE, IGNORE);
}

void condition_init(condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_INIT, (int)condition, IGNORE, IGNORE);
}

void condition_wait(mutex_lock_t *lock, condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_WAIT, (int)lock, (int)condition, IGNORE);
}

void condition_signal(condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_SIGNAL, (int)condition, IGNORE, IGNORE);
}

void condition_broadcast(condition_t *condition)
{
    invoke_syscall(SYSCALL_CONDITION_BROADCAST, (int)condition, IGNORE, IGNORE);
}

void sys_move_cursor(int x, int y)
{
    invoke_syscall(SYSCALL_CURSOR, x, y, IGNORE);
}

void mutex_lock_init(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_INIT, (int)lock, IGNORE, IGNORE);
}

void mutex_lock_acquire(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_ACQUIRE, (int)lock, IGNORE, IGNORE);
}

void mutex_lock_release(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_RELEASE, (int)lock, IGNORE, IGNORE);
}






void sys_init_mac(void)
{
    invoke_syscall(SYSCALL_INIT_MAC, IGNORE, IGNORE,IGNORE);
}

void sys_net_send(uint32_t td, uint32_t td_phy)
{
    invoke_syscall(SYSCALL_NET_SEND, (int)td, (int)td_phy, IGNORE);
}

uint32_t sys_net_recv(uint32_t rd, uint32_t rd_phy, uint32_t daddr)
{
    invoke_syscall(SYSCALL_NET_RECV, (int)rd, (int)rd_phy, (int)daddr);
}

void sys_wait_recv_package(void)
{
    invoke_syscall(SYSCALL_WAIT_RECV, IGNORE, IGNORE, IGNORE);
}




int sys_mkfs(void)
{
    invoke_syscall(SYSCALL_MKFS, IGNORE, IGNORE, IGNORE);
}

int sys_mkdir(char *dir_name)
{
    invoke_syscall(SYSCALL_MKDIR, (int)dir_name, IGNORE, IGNORE);
}

int sys_rmdir(char *dir_name)
{
    invoke_syscall(SYSCALL_RMDIR, (int)dir_name, IGNORE, IGNORE);    
}

int sys_read_dir(char *path)
{
    invoke_syscall(SYSCALL_READDIR, (int)path, IGNORE, IGNORE);
}

int sys_fs_info(void)
{
    invoke_syscall(SYSCALL_FSINFO, IGNORE, IGNORE, IGNORE);
}

int sys_enter_fs(char *path)
{
    invoke_syscall(SYSCALL_ENTERFS, (int)path, IGNORE, IGNORE);
}



int sys_ftouch(char *fname)
{
    invoke_syscall(SYSCALL_FTOUCH, (int)fname, IGNORE, IGNORE);
}

int sys_fcat(char *fname)
{
    invoke_syscall(SYSCALL_FCAT, (int)fname, IGNORE , IGNORE);
}

int sys_fopen(char *fname, int access)
{
    invoke_syscall(SYSCALL_FOPEN, (int)fname, access, IGNORE);
}

int sys_fclose(int fd)
{
    invoke_syscall(SYSCALL_FCLOSE, fd, IGNORE, IGNORE);
}

int sys_fread(int fd, char *buff, int size)
{
    invoke_syscall(SYSCALL_FREAD, fd, (int)buff, size);
}

int sys_fwrite(int fd, char *buff, int size)
{
    invoke_syscall(SYSCALL_FWRITE, fd, (int)buff, size);
}

int sys_ffind(char *path, char *fname)
{
    invoke_syscall(SYSCALL_FFIND, (int)path, (int)fname, IGNORE);
}

int sys_frename(char *old_fname, char *new_fname)
{
    invoke_syscall(SYSCALL_FRENAME, (int)old_fname, (int)new_fname, IGNORE);
}

int sys_fln(char *target, char *linkname, char *para)
{
    invoke_syscall(SYSCALL_FLN, (int)target, (int)linkname, (int)para);
}

int sys_fseek(int fd, uint32_t offset)
{
    invoke_syscall(SYSCALL_FSEEK, fd, (int)offset, IGNORE);
}


int sys_info(void)
{
    invoke_syscall(SYSCALL_INFO, IGNORE,IGNORE,IGNORE);
}



