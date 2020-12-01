#include "sched.h"
#include "stdio.h"
#include "test2.h"
#include "syscall.h"

static char blank[] = {"                    "};
static char plane1[] = {"    ___         _   "};
static char plane2[] = {"| __\\_\\______/_|  "};
static char plane3[] = {"<[___\\_\\_______|  "};
static char plane4[] = {"|  o'o              "};

void printf_task1(void)
{
    int i;
    int print_location = 7;

    for (i = 0;; i++)
    {
        sys_move_cursor(1, print_location);
        //CloseInt();
        printf("> [TASK] This task is to test scheduler. (%d)               ", i);
        //SetInt();
    }
}

void printf_task2(void)
{
    int i;
    int print_location = 8;

    for (i = 0;; i++)
    {
        sys_move_cursor(1, print_location);
        //CloseInt();
        printf("> [TASK] This task is to test scheduler. (%d)               ", i);
        //SetInt();
    }
}

void drawing_task2(void)
{
    int i, j = 22;

    while (1)
    {
        for (i = 55; i > 0; i--)
        {
            sys_move_cursor(i, j + 0);
            //CloseInt();
            printf("%s", plane1);
            //SetInt();

            sys_move_cursor(i, j + 1);
            //CloseInt();
            printf("%s", plane2);
            //SetInt();

            sys_move_cursor(i, j + 2);
            //CloseInt();
            printf("%s", plane3);
            //SetInt();

            sys_move_cursor(i, j + 3);
            //CloseInt();
            printf("%s", plane4);
            //SetInt();
        }

        sys_move_cursor(1, j + 0);
        //CloseInt();
        printf("%s", blank);
        //SetInt();

        sys_move_cursor(1, j + 1);
        //CloseInt();
        printf("%s", blank);
        //SetInt();

        sys_move_cursor(1, j + 2);
        //CloseInt();
        printf("%s", blank);
        //SetInt();

        sys_move_cursor(1, j + 3);
        //CloseInt();
        printf("%s", blank);
        //SetInt();
    }
}