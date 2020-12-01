#include <stdio.h>
int func(int p);
int a=5;
int b=0;
int main(){
	int c;
	c=1;
	b=func(a+c);
	printf("b=%d\n",b);
	return 0;
}

int func(int p){
	int a=1,b,c;
	{
		int a=10;
		b=++a;
	}
	a+=100;
	c=a+b+p;
	return c;
}
