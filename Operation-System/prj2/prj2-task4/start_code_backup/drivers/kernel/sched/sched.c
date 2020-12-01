#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"

pcb_t pcb[NUM_MAX_TASK];

/* current running task PCB */
pcb_t *current_running;
pcb_t *sche_item;
/* global process id */
pid_t process_id = 1;

static void check_sleeping(void)
{
    pcb_t *p = sleep_queue.head;
    uint32_t time = get_timer();

    while(!queue_is_empty(&sleep_queue) && (p != NULL))
    {
        if(p->sleep_time <= time - p->begin_time)
        {
            pcb_t *q;
            q = (pcb_t *)queue_remove(&sleep_queue, (void *)p);
            p->sleep_time = 0;
            p->status = TASK_READY;
            queue_push(&ready_queue, (void *)p);
            p = q;
        }
        else
            p = p->next;
    }
}

void scheduler(void)
{
    // TODO schedule
    // Modify the current_running pointer.
    check_sleeping();

    if(current_running->status == TASK_RUNNING)
    {
        current_running->status = TASK_READY;
        queue_push(&ready_queue,current_running);

        if(!queue_is_empty(&ready_queue))
        {
            current_running = queue_dequeue(&ready_queue);
            current_running->status = TASK_RUNNING;
        }
    }
    else if(current_running->status == TASK_BLOCKED)
    {
        if(!queue_is_empty(&ready_queue))
        {
            current_running = queue_dequeue(&ready_queue);
            current_running->status = TASK_RUNNING;
        }       
    }  
}

void do_sleep(uint32_t sleep_time)
{
    // TODO sleep(seconds)
    current_running->status = TASK_BLOCKED;
    current_running->begin_time = get_timer();
    current_running->sleep_time = sleep_time;

    queue_push(&sleep_queue,current_running);
    do_scheduler();
    //while(1)
    //    ;
}

void do_other(void)
{
    // TODO Nothing
}

void do_block(queue_t *queue)
{
    // block the current_running task into the queue
    current_running->status = TASK_BLOCKED;
    queue_push(queue,current_running);

}

void do_unblock_one(queue_t *queue)
{
    // unblock the head task from the queue
    pcb_t *item;
    if(!queue_is_empty(queue))
    {
        item = queue_dequeue(queue);
        item->status = TASK_READY;
        queue_push(&ready_queue,item);   
    }  
}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
    pcb_t *item;
    while(!queue_is_empty(queue))
    {
        item = queue_dequeue(queue);
        item->status = TASK_READY;
        queue_push(&ready_queue,item);   
    } 
}

void CloseInt(void)
{
    int cp0_status;
    cp0_status = GET_CP0_STATUS();
    cp0_status &= 0xfffffffe;
    SET_CP0_STATUS(cp0_status);
}

void SetInt(void)
{
    int cp0_status;
    cp0_status = GET_CP0_STATUS();
    cp0_status |= 0x1;
    SET_CP0_STATUS(cp0_status);
}
