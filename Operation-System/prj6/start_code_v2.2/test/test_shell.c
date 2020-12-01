/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                  The shell acts as a task running in user mode. 
 *       The main function is to make system calls through the user's output.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include "test.h"
#include "stdio.h"
#include "screen.h"
#include "syscall.h"
#include "fs.h"

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

struct task_info task1 = {"task1", (uint32_t)&test_fs, USER_PROCESS};

static struct task_info *test_tasks[16] = {&task1};
static int num_test_tasks = 1;

int do_cmd(char cmd[20], int argc, char argv[10][100])
{
    switch (argc)
    {
    case 1:
        if(!strcmp(cmd,"ps"))
        {
            sys_ps();
        }
        else if(!strcmp(cmd,"clear"))
        {
            sys_clear();
        }
        else if(!strcmp(cmd,"exit"))
        {
            sys_exit();
        }
        else if(!strcmp(cmd,"mkfs"))
        {
            sys_mkfs();              
        }
        else if(!strcmp(cmd,"statfs"))
        {
            sys_fs_info();
        }
        else if (!strcmp(cmd,"ls"))
        {
            char path[2] = "\0";
            sys_read_dir(path);
        }
        else if (!strcmp(cmd,"info"))
        {
            sys_info();
        }          
        else
        {
            printf("Unknown Command\n");
        }      
        break;

    case 2:
        if(!strcmp(cmd,"clear") && !strcmp(argv[0],"all"))
        {
            sys_clear_all();
        }
        else if(!strcmp(cmd,"exec"))
        {
            int i = atoi(argv[0]);
            sys_spawn(test_tasks[i]);
        }
        else if(!strcmp(cmd,"kill"))
        {
            int i = atoi(argv[0]);
            sys_kill(i);            
        }
        else if(!strcmp(cmd,"cd"))
        {
            char *path = (char *)(&argv[0]);
            sys_enter_fs(path);
        }
        else if (!strcmp(cmd,"mkdir"))
        {
            char *dir_name = (char *)(&argv[0]);
            sys_mkdir(dir_name);
        }
        else if(!strcmp(cmd,"rmdir"))
        {
            char *dir_name = (char *)(&argv[0]);
            sys_rmdir(dir_name);            
        }
        else if (!strcmp(cmd,"ls"))
        {
            char *path = (char *)(&argv[0]);
            sys_read_dir(path);
        }
        else if(!strcmp(cmd,"touch"))
        {
            char *fname = (char *)(&argv[0]);
            sys_ftouch(fname); 
        }
        else if(!strcmp(cmd,"cat"))
        {
            char *fname = (char *)(&argv[0]);
            sys_fcat(fname); 
        }
        else
        {
            printf("Unknown Command\n");
        }
        break;

    default:
            printf("Unkonwn Command\n");
        break;
    }

}

void analysis_cmd(char cmd[20], int *argc, char argv[10][100])
{
    int i = 0;
    int j = 0;
    int k = 0;
    while(cmd[i] != ' ' && cmd[i] != '\0')
        i++;
    if(cmd[i] == ' ')
    {
        cmd[i] = '\0';
        i++;
    }
    *argc = 1;

    while(cmd[i])
    {
        k = 0;
        do{
            argv[j][k++] = cmd[i++];
        }while(cmd[i] != ' ');    
        argv[j][k] = '\0';
        (*argc)++;
        i++;
        j++;
    }
}

void test_shell()
{
    char cmd[20];
    int i = 0;

    int argc;
    char argv[10][100];

    // terminal
    sys_move_cursor(0,SCREEN_HEIGHT / 2);
    printf("------------------------------------TERMINAL-----------------------------------\n");
    printf("->root@ZL_OS:");printf("%s$ ",Current_Direction->current_path);

    // TODO solve command
    while (1)
    {
        // read command from UART port
        disable_interrupt();
        char ch = read_uart_ch();
        enable_interrupt();           

        if(ch ==0)
            continue;

        if(ch != '\r')
        {
            cmd[i++] = ch;
            printf("%c",ch); 
//            printf("%c\n",cmd[i-1]);
        }
        else
        {
            cmd[i++] = ' ';
            cmd[i] = '\0';
            analysis_cmd(cmd, &argc, argv);

            printf("\n");
//            printf("\ncmd: %s\n",cmd);
            do_cmd(cmd, argc, argv);

            i = 0;
            printf("->root@ZL_OS:");printf("%s$ ",Current_Direction->current_path);
        }      

    }
}