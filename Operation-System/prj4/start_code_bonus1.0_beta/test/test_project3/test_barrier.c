#include "time.h"
#include "stdio.h"
#include "syscall.h"
#include "barrier.h"
#include "test3.h"

static barrier_t barrier;
static int is_init = 0;
static int num_task = 3;

void barrier_task1(void)
{
    int i;
    int print_location = 0;

    if (!is_init)
    {
        is_init = 1;
        barrier_init(&barrier, num_task);
    }

    for (i = 0;; i++)
    {
        sys_move_cursor(0, print_location);
        printf("> [TASK] Ready to enter the barrier.(%d)", i);

        barrier_wait(&barrier);

        sys_move_cursor(0, print_location);
        printf("> [TASK] Exited barrier (%d).           ", i);

        sys_sleep(1);
    }
}

void barrier_task2(void)
{
    int i;
    int print_location = 1;

    if (!is_init)
    {
        is_init = 1;
        barrier_init(&barrier, num_task);
    }

    for (i = 0;; i++)
    {
        sys_move_cursor(0, print_location);
        printf("> [TASK] Ready to enter the barrier.(%d)", i);

        barrier_wait(&barrier);

        sys_move_cursor(0, print_location);
        printf("> [TASK] Exited barrier (%d).           ", i);

        sys_sleep(1);
    }
}

void barrier_task3(void)
{
    int i;
    int print_location = 2;

    if (!is_init)
    {
        is_init = 1;
        barrier_init(&barrier, num_task);
    }

    for (i = 0;; i++)
    {
        sys_move_cursor(0, print_location);
        printf("> [TASK] Ready to enter the barrier.(%d)", i);

        barrier_wait(&barrier);

        sys_move_cursor(0, print_location);
        printf("> [TASK] Exited barrier (%d).           ", i);

        sys_sleep(1);
    }
}