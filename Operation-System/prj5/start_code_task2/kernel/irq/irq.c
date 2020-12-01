#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"
#include "mac.h"

queue_t recv_block_queue;
desc_t *Handle_Recv_Desc;
uint32_t Recv_count;
uint32_t recv_flag[PNUM];

void other_exception_handler(void);

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.

    screen_reflush();
    time_elapsed += 150000;

    if(!queue_is_empty(&recv_block_queue) & !(RX_Desc[PNUM-1].tdes0 & 0x80000000))
    {
        int i;
        for(i = 0; i < PNUM; i++)
            recv_flag[i] = 1;
            
        do_unblock_one(&recv_block_queue);
    }

    do_scheduler();
}

void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
    uint32_t IP;
    IP = (cause & 0x0000ff00) >> 8;

    if(IP == 0x80)
        irq_timer();
    else
        other_exception_handler();
}

void other_exception_handler(void)
{
    // TODO other exception handler
    time_elapsed += 150000;
}