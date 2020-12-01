#include <stdio.h>

int main(){
	int a,b,c;
	a=b=5;
	c=a++,a+b,b+=3;
	printf("a=%d b=%d c=%d\n",a,b,c);
	a=b=5;
	c=(a++,a+b,b+=3);
	printf("a=%d b=%d c=%d\n",a,b,c);
	return 0;
}
