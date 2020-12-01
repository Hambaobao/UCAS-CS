#include "barrier.h"
#include "sched.h"

void do_barrier_init(barrier_t *barrier, int goal)
{
    barrier->barrier_goal = goal;
    barrier->barrier_waiting_number = 0;
    queue_init(&barrier->barrier_queue);
}

void do_barrier_wait(barrier_t *barrier)
{
    barrier->barrier_waiting_number++;
    if(barrier->barrier_waiting_number < barrier->barrier_goal)
    {
        do_block(&barrier->barrier_queue);
    }
    else
    {
        do_unblock_all(&barrier->barrier_queue);

        do_scheduler();
    }  
}