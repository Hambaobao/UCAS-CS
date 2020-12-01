#include <stdio.h>
int fib(int );
int main(){
	int n,d;
	int fib(int );
	scanf("%d",&n);
	d=fib(n);
	printf("F(n)=%d\n",d);
	printf("\n");
	for(int i=0;i<=n;i++)
		printf("F(%d)=%d\n",i,fib(i));
	return 0;
}

int fib(int n){
	if(n==0)
		return 0;
	else if(n==1)
		return 1;
	else
		return fib(n-1)+fib(n-2);

}
