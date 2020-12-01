#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"
#include "mac.h"

queue_t recv_block_queue;
desc_t *Handle_Recv_Desc;
uint32_t Recv_count;

void other_exception_handler(void);

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.

    screen_reflush();
    time_elapsed += 150000;
        
    do_scheduler();
}

int mac_irq_count = 0;
int int_count = 0;
void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
    uint32_t IP;
    IP = (status & cause & 0x0000ff00) >> 8;

    vt100_move_cursor(1, 4);
    printk("int_count %d",int_count);
    int_count++;

    if(IP == 0x80)
        irq_timer();
    else if(IP == 0x8)
    {
        if(reg_read_32(INT1_SR_ADDR) & 0x8)
        {
            vt100_move_cursor(1, 6);
            printk("mac_irq here %d",mac_irq_count);
            mac_irq_count++;

            mac_irq_handle();
        }
    }
    else
        other_exception_handler();
}

void other_exception_handler(void)
{
    // TODO other exception handler
    time_elapsed += 150000;
}