#include "lock.h"
#include "sched.h"
#include "syscall.h"

void spin_lock_init(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void spin_lock_acquire(spin_lock_t *lock)
{
    while (LOCKED == lock->status)
    {
    };
    lock->status = LOCKED;
}

void spin_lock_release(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void do_mutex_lock_init(mutex_lock_t *lock)
{
    lock->status = UNLOCKED;
    queue_init(&(lock->wait_queue));
}

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
    if(lock->status == UNLOCKED)
    {
        queue_push(&(lock->wait_queue),current_running);
        lock->status = LOCKED;
    }
    else if((lock->wait_queue).head != current_running)
    {
        if(!is_in_queue(&(lock->wait_queue),current_running))
        {
            queue_push(&(lock->wait_queue),current_running);
            do_block(&(lock->block_queue));

            do_scheduler();

        }
    }
//    do_scheduler();   
}

void do_mutex_lock_release(mutex_lock_t *lock)
{
    if(queue_is_empty(&(lock->wait_queue)))
    {
        lock->status = UNLOCKED;
    }
    else
    {
        queue_dequeue(&(lock->wait_queue));//head leave
        if(queue_is_empty(&(lock->wait_queue)))
        {
            lock->status = UNLOCKED;
        }
    }
    
    do_unblock_all(&(lock->block_queue));
}
