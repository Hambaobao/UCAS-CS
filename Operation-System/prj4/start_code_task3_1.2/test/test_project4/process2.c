#include "sched.h"
#include "stdio.h"
#include "syscall.h"
#include "time.h"
#include "screen.h"
#include "test4.h"

#define RW_TIMES 3

int rand()
{	
	int current_time = GET_CP0_COUNT();
	return current_time % 100000;
}

static void disable_interrupt()
{
    uint32_t cp0_status = GET_CP0_STATUS();
    cp0_status &= 0xfffffffe;
    SET_CP0_STATUS(cp0_status);
}

static void enable_interrupt()
{
    uint32_t cp0_status = GET_CP0_STATUS();
    cp0_status |= 0x01;
    SET_CP0_STATUS(cp0_status);
}

static char read_uart_ch(void)
{
    char ch = 0;
    unsigned char *read_port = (unsigned char *)(0xbfe48000 + 0x00);
    unsigned char *stat_port = (unsigned char *)(0xbfe48000 + 0x05);

    while ((*stat_port & 0x01))
    {
        ch = *read_port;
    }
    return ch;
}

void scanf(int *n)
{
    char str[20];
    int i = 0;

	int temp;

	disable_interrupt();
	screen_reflush();	

	vt100_move_cursor(0,3);	
	printk("                             ");
	vt100_move_cursor(0,3);	

	while (1)
    {
        // read command from UART port
        char ch = read_uart_ch();       

        if(ch == 0)
            continue;

        if(ch != '\r')
        {
            str[i++] = ch;
            printk("%c",ch); 
        }
        else
        {
            str[i++] = ' ';
            str[i] = '\0';
			temp = atox(str);
			*n = temp;
			break;
        }      
    }
	enable_interrupt();    
}

void rw_task1()
{
	int mem1, mem2 = 0;
	int curs = 4;
	int memory[RW_TIMES];
	int i = 0;
	for(i = 0; i < RW_TIMES; i++)
	{
		sys_move_cursor(0, 1);
		printf("input write address %d:  ",i);

		scanf(&mem1);

		sys_move_cursor(0, curs+i);
		memory[i] = mem2 = rand();

		*(int *)mem1 = mem2;
		printf("-> Write: 0x%x, %d", mem1, mem2);
	}
	curs += RW_TIMES;
	for(i = 0; i < RW_TIMES; i++)
	{
		sys_move_cursor(0, 1);
		printf("input read address %d:  ",i);
		scanf(&mem1);
		sys_move_cursor(0, curs+i);

		memory[i+RW_TIMES] = *(int *)mem1;

		if(memory[i+RW_TIMES] == memory[i])
			printf("-> Read succeed: %d, address: 0x%x", memory[i+RW_TIMES],mem1);
		else
			printf("-> Read error: %d, addrress: 0x%x", memory[i+RW_TIMES],mem1);
	}
	while(1);
	//Input address from argv.
}
