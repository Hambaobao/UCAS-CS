#include <stdio.h>
int main(){
	int power (int base,int p);
	for (int j=1;j<=10;j++)
	printf("%d %d %d\n",j,power(2,j),power(-3,j));
	return 0;
}
int power (int base,int p)
{	int c=1;	
	for (int i=1;i<=p;i++)
	c=c*base;
	return c;
}
