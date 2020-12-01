#include <stdio.h>
//回显命令行参数：版本2
int main(int argc,char *argv[]){
	while(--argc>0)
		printf("%s%s",*++argv,(argc>1)?" ":"");
	printf("\n");
	return 0;
}
