#include <stdio.h>

#define N 123

int main(){
	int i;
	for (i=0;i<N;i++)
		printf("%6d%c",i,(i%10==9||i==N-1)?'\n':' ');
	return 0;}
