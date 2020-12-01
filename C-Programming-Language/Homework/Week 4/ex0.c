#include<stdio.h>
int main(){
	int n;
	int isprime(int n);
	n=100;
	do{
		if(isprime(n)==1)
			printf("%d is prime\n",n);
		n=n+1;
	}while(n<=1000);
	return 0;
}
int isprime(int n){
	int i,r;
	if(n<=1)
		return 0;
	else if(n==2)
		return 1;
	i=2;
	do{
		r=n%i;
		if(r==0)
			return 0;
		i=i+1;
	}while(i<n);
	return 1;
}
