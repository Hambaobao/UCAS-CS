#include <stdio.h>
int main(){
	int isPrime(int i);
	int i;
	for(i=100;i<=10000;i++)
		if (isPrime(i)==1)
		printf("%d is prime\n",i);
	return 0;
}
int isPrime(int i){
	int j;	
	for (j=2;j<i;j++)
		if (i%j==0)
			return 0;
	return 1;
}
