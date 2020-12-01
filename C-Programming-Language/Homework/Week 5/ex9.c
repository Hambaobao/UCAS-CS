#include <stdio.h>
int main(){
	int org;
	int a,b;
	org=0x12345678;
	a=(char)org;
	b=(short)org;
	printf("1: a=%x,b=%x\n",a,b);
	org=0x123456FF;
	a=(char)org;
	b=(short)org;
	printf("2: a=%x,b=%x\n",a,b);
	return 0;
}
