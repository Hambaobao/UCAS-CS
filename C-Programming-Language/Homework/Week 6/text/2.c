#include <stdio.h>
int main()
{	int wn(int x);
	for(int n=1;n<1000;n++)
	{if(wn(n)==1)
		printf("%d\n",n);}
	return 0;
}
int wn(int x)
{	int i,sum;
	sum=0;
	for(i=1;i<x;i++)
	{	if(x%i==0)	
			sum+=i;}
	if (sum==x)
		return 1;
	else	
		return 0;
}
