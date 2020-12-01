#include <stdio.h>

int prod(int );

int main(){
	for(int n=0;n<=10;n++)
		{
			for(int i=0;i<=n;i++)
				printf("%d\t",prod(n)/prod(n-i)/prod(i));
				printf("\n");
						}
	return 0;
}

int prod(int n)
{	int p,i;
	p=1;
	i=1;
	do{
		p=p*i;
		i=i+1;
	}while(i<=n);
	return p;
}
