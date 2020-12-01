#include <stdio.h>
int main()
{
	int max(int x, int y);
	int a, b, c;
	scanf("%d, %d",&a, &b);
	c=max(a,b);
	printf("max=%d\n",c);
	return 0;
}
int max(int x, int y)
{
	int z;
	if(x>y)
		z=x;
	else
		z=y;
	return z;
}