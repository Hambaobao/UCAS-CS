#include <stdio.h>
int main(){
	int power (int base,int p);
	int m,n;
	scanf("%d %d",&m,&n);
	for (int j=1;j<=n;j++)
	printf("%d's %d power is %d\n",m,j,power(m,j));
	return 0;
}
int power (int base,int p)
{	int c=1;	
	for (int i=1;i<=p;i++)
	c=c*base;
	return c;
}
