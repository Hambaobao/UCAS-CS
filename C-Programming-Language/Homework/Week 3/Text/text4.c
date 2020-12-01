#include <stdio.h>
int main()
{	float prod (int);
	float sum;
	int i,n;
	scanf("%d",&n);
	sum=0;
	for(i=0;i<=n;i++)
	sum=sum+prod(i);
	printf("sum is %f\n",sum);
	return 0;
}
float prod(int n)
{	float demo,p;
	int j;
	demo=1;
	for(j=1;j<=n;j++)
	demo=demo*j;
	p=1/demo;
	return p;
}
	
