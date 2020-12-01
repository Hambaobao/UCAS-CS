#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"

extern int screen_cursor_x;
extern int screen_cursor_y;

void other_exception_handler(void);

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.
//    SET_CP0_COUNT(0);
//    SET_CP0_COMPARE(0x30000);

    screen_reflush();

    time_elapsed += 150000;
    
    current_running->cursor_x = screen_cursor_x;
    current_running->cursor_y = screen_cursor_y;

    do_scheduler();

    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;

//    screen_clear();
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