#include <stdio.h>
#define BUFSIZE 100

int error;

char buf[BUFSIZE];//用于ungetch函数缓冲区
int bufp=0;       //buf中的下一个空闲位置
int getch(void){  //取一个字符（可能是压回的字符）
	return (bufp>0)?buf[--bufp]:getchar();
}

void ungetch(int c){//把字符压回到输入中
	if(bufp>=BUFSIZE)
	{
		printf("ungetch: too many characters\n");
		error=1;
	}
	else
		buf[bufp++]=c;
}
