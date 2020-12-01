#include <stdio.h>
int fib(int );
int main(){
	int n,d;
	int fib(int );
	printf("input the number of stairs:");
	scanf("%d",&n);
	d=fib(n);
	printf("the total number of ways to finish is %d\n",d);
	return 0;
}

int fib(int n){
	if(n==0)
		return 1;
	else if(n==1)
		return 1;
	else
		return fib(n-1)+fib(n-2);

}
