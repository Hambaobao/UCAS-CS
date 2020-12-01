#include <stdio.h>
int main()
{	int a,b,m,n;
	int gcd(int ,int );
	int lcm(int ,int );
	scanf("%d %d",&m,&n);
	a=gcd(m,n);
	b=lcm(m,n);
	printf("the gcd is %d,the lcm is %d\n",a,b);
	return 0;
}
int gcd(int m,int n)
{	int r;
	do{r=m%n;
		m=n;
		n=r;}
		while(r!=0);
	return m;
}
int lcm(int m,int n)
{	int gcd(int m,int n);
	int z,d;
	d=gcd(m,n);
	z=(m*n)/d;
	return z;
}
