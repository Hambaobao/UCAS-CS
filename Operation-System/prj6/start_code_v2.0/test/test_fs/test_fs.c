#include "fs.h"
#include "stdio.h"
#include "string.h"
#include "test_fs.h"
#include "syscall.h"

static char buff[64];

void test_fs(void)
{
    int i, j;
    int fd = sys_fopen("1.txt", O_RDWR);

    for (i = 0; i < 10; i++)
    {
        sys_fwrite(fd, "hello world!\n", 13);
    }

    for (i = 0; i < 10; i++)
    {
        sys_fread(fd, buff, 13);
        for (j = 0; j < 13; j++)
        {
            printf("%c", buff[j]);
        }
    }

    sys_fclose(fd);
    sys_exit();
}