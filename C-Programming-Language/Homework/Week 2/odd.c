#include <stdio.h>
int main()
{	int prod(int n);
	int product;
	int n;
	scanf("%d",&n);
	product=prod(n);
	printf("the product is %d\n",product);
	return 0;
}
int prod(int n)
{	int p,i;
	p=1;
	i=1;
	do{
		p=p*i;
		i=i+2;
	}while(i<=n);
	return p;
}
