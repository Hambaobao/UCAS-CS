#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"
#include "mm.h"

pcb_t pcb[NUM_MAX_TASK];

/* current running task PCB */
pcb_t *current_running;
pcb_t *sche_item;
/* global process id */
pid_t process_id = 1;

uint32_t stack_top = 0xa0f00000;
uint32_t kernel_stack_top = 0xa0f00000;
uint32_t user_stack_top = 0xfff000; // 16M - 4K

int exit_process_top = 0;

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
            priority_queue_push((void *)p);
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

    current_running->cursor_x = screen_cursor_x;
    current_running->cursor_y = screen_cursor_y;

    if(current_running->status == TASK_RUNNING)
    {
        current_running->status = TASK_READY;
        priority_queue_push(current_running);

        pcb_t * temp;
        do{
            temp = (pcb_t *)priority_queue_dequeue();
        }while(temp->status != TASK_READY);
        current_running = temp;       
        current_running->status = TASK_RUNNING;
    }

    else
    {
        current_running = (pcb_t *)priority_queue_dequeue();
        current_running->status = TASK_RUNNING;     
    }

    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;  
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

    do_scheduler();
}

void do_unblock_one(queue_t *queue)
{
    // unblock the head task from the queue
    pcb_t *item;
    if(!queue_is_empty(queue))
    {
        item = queue_dequeue(queue);
        item->status = TASK_READY;
        priority_queue_push(item);   
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
        priority_queue_push(item);   
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

pid_t do_getpid(void)
{
    return current_running -> pid;
}

void task_status_print(int i)
{
    switch (i)
    {
    case 0:
        my_printf("TASK_BLOCKED "); break;
    case 1:
        my_printf("TASK_RUNNING "); break;
    case 2:
        my_printf("TASK_READY   "); break;
    case 3:
        my_printf("TASK_EXITED  "); break;
    case 4:
        my_printf("TASK_WAIT    "); break;    
    default:
        break;
    }
}

/*
void do_ps(void)
{
    my_printf("[PROCESS TABLE]\n");
    my_printf("[%d] ",0);
    my_printf("PID : %d    ",current_running->pid);
    my_printf("STATUS : ");task_status_print(current_running->status);
    my_printf("   PRIORITY : %d ",current_running->priority);
    my_printf("\n");
    
    int i;
    int j = 1;
    for(i = 0;i < MAXPRIORITY;i++)
    {
        if(queue_is_empty(ready_queue + i))
            continue;
        else
        {
            pcb_t *item = (pcb_t *)ready_queue[i].head;
            do{
                my_printf("[%d] ",j++);
                my_printf("PID : %d    ",item->pid);
                my_printf("STATUS : ");task_status_print(item->status);
                my_printf("   PRIORITY : %d ",item->priority);
                my_printf("\n");

                item = item->next;
            }while(item);
        }

        if(queue_is_empty(&block_queue))
            continue;
        else
        {
            pcb_t *item = (pcb_t *)block_queue.head;
            do{
                my_printf("[%d] ",j++);
                my_printf("PID : %d    ",item->pid);
                my_printf("STATUS : ");task_status_print(item->status);
                my_printf("   PRIORITY : %d ",item->priority);
                my_printf("\n");

                item = item->next;
            }while(item);
        }

        if(queue_is_empty(&sleep_queue))
            continue;
        else
        {
            pcb_t *item = (pcb_t *)sleep_queue.head;
            do{
                my_printf("[%d] ",j++);
                my_printf("PID : %d    ",item->pid);
                my_printf("STATUS : ");task_status_print(item->status);
                my_printf("   PRIORITY : %d ",item->priority);
                my_printf("\n");

                item = item->next;
            }while(item);
        }
                    
    }
}
*/

void do_ps()
{
    int i;
    for(i = 0; i < NUM_MAX_TASK;i++)
    {
        if(pcb_valid[i])
        {
            my_printf("[%d] ",i);
            my_printf("PID : %d    ",pcb[i].pid);
            my_printf("STATUS : ");task_status_print(pcb[i].status);
            my_printf("   PRIORITY : %d ",pcb[i].priority);
            my_printf("\n");        
        }
    }    
}

void do_clear()
{
    screen_clear(SCREEN_HEIGHT / 2 + 1, SCREEN_HEIGHT);
    screen_move_cursor(0, SCREEN_HEIGHT / 2 + 1);
}

void do_clear_all()
{
    screen_clear(0, SCREEN_HEIGHT);
    screen_move_cursor(0, SCREEN_HEIGHT / 2);
    my_printf("------------------------------------TERMINAL-----------------------------------\n");
}

extern mutex_lock_t lock1;
extern mutex_lock_t lock2;

void recycle(pid_t pid)
{
    int i;
    for(i = 0; i < NUM_MAX_TASK; i++)
    {
        if(pcb[i].pid == pid)
            break;
    }

    if(i == NUM_MAX_TASK)
    {
        my_printf("Wrong pid\n");
        return ;
    }

    if(&pcb[i] == lock1.wait_queue.head)
    {
        do_mutex_lock_release(&lock1);
    }

    if(&pcb[i] == lock2.wait_queue.head)
    {
        do_mutex_lock_release(&lock2);
    }

    do_unblock_all(&(pcb[i].wait_queue));

    pcb[i].status = TASK_EXITED;
    pcb_valid[i] = 0;
}

void do_spawn(task_info_t *task, int *a)
{
    int i = 0;
    while(pcb_valid[i] && i < NUM_MAX_TASK)
        i++;
    
    if(i == NUM_MAX_TASK)
    {
        my_printf("There is no space for more process\n");
        return ;
    }

    pcb_valid[i] = 1;

    pcb[i].user_context.regs[4] = a;

	pcb[i].pid = process_id++;
	pcb[i].type = task->type;
	pcb[i].status = TASK_READY;

	pcb[i].prev = NULL;		
	pcb[i].next = NULL;

	pcb[i].kernel_context.regs[31] = (uint32_t)&init_handle;
	pcb[i].user_context.regs[31] = task->entry_point;

	pcb[i].kernel_context.cp0_epc = task->entry_point;
	pcb[i].user_context.cp0_epc = task->entry_point;

	pcb[i].kernel_context.cp0_status = 0x10008003;
	pcb[i].user_context.cp0_status = 0x10008003;

	pcb[i].kernel_context.regs[29] = pcb[i].kernel_stack_top;
	pcb[i].user_context.regs[29] = pcb[i].user_stack_top;

    pcb[i].priority = 1;
	priority_queue_push(&pcb[i]);
}

void do_kill(pid_t pid)
{
    recycle(pid);
}

void do_exit(void)
{
    current_running->status = TASK_EXITED;

    recycle(current_running->pid);    

    do_scheduler();
}

void do_wait(pid_t pid)
{
    int i;
    for(i = 0; i < NUM_MAX_TASK; i++)
    {
        if(pcb[i].pid == pid)
            break;
    }

    if(i == NUM_MAX_TASK)
    {
        my_printf("Waiting Task Does Not exist\n");
        return ;
    }

    current_running->status = TASK_WAIT;
    queue_push(&(pcb[i].wait_queue),current_running);
    
    do_scheduler();  
}

