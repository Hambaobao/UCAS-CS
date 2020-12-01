#include <stdio.h>
int main()
{
	int caculate(int x, int y);
	int a, b, c;
	a=10;
	b=8;
	c=caculate(a,b);
	printf("c=%d\n",c);
	a=7;
	b=12;
	c=caculate(a,b);
	printf("c=%d\n",c);
	return 0;
}

int caculate(int x, int y)
{
	int z;
	if(x>y)
		z=x-y;
	else
		z=x*y;
	return z;
}
