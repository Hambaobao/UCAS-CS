#include "sem.h"
#include "stdio.h"
#include "sched.h"

void do_semaphore_init(semaphore_t *s, int val)
{
    if(val >= 0)
    {
        s->sem = val;
        queue_init(&(s->sem_queue));
    }
    else
    {
        my_printf("value error\n");
    }  
}

void do_semaphore_up(semaphore_t *s)
{
    if(!queue_is_empty(&s->sem_queue))
    {
        pcb_t * item = (pcb_t *)priority_queue_dequeue(&s->sem_queue);
        item->status = TASK_READY;
        priority_queue_push(&ready_queue, (void *)item);
    }
    else
    {
        s->sem++;  
    }
}

void do_semaphore_down(semaphore_t *s)
{
    if(s->sem)
    {
        s->sem--;
    }
    else
    {
        do_block(&s->sem_queue);
    }
}