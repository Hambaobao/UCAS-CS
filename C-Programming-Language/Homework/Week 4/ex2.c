#include <stdio.h>
int main(){
	int intvar;
	intvar=2147483647;
	printf("1 intvar=%d\n",intvar);
	
	intvar=2147483647l*3/3;
	printf("2 intvar=%d\n",intvar);
	return 0;
}
