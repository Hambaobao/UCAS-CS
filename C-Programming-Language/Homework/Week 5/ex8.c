#include <stdio.h>

int main(){
	char c;
	unsigned char uc;
	int a,b;
	c=0xFF;
	uc=c;
	a=c;
	b=uc;
	printf("a=%d,b=%d\n",a,b);
	return 0;
}
