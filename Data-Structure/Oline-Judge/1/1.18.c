#include <stdio.h>

#define MAXINT 46080

//i!*2^i

int main(){
	int fact(int);
	int power(int);
	int arrsize=300;
	int array[arrsize];
	int n;
	scanf("%d",&n);

//	int m;
//	scanf("%d",&m);
//	printf("%d %d\n",fact(n),power(n));

	for(int i=0;i<arrsize;i++)
		array[i]=0;

	if(n>arrsize || n<=0)
		return MAXINT;
	
	for(int i=1;i<n;i++)
	{
		int m;
		m=fact(i)*power(i);
		if(m<MAXINT && m>array[i-1])
			array[i]=m;
		else 
			array[i]=MAXINT;
	}

	if(array[n-1]<MAXINT)
		printf("%d\n",array[n-1]);
	else 
		printf("%d\n",MAXINT);

	return 0;
}

int fact(int i){
	if(i==0)
		return 0;
	int m=i;
	while(i>1){
		m*=(i-1);
		i--;
	}
	return m;
}

int power(int i){
	int m=1;
	if(i==0)
		return 1;
	while(i>0){
		m*=2;
		i--;
	}
	return m;	
}
