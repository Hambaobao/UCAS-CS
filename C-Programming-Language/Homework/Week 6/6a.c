#include <stdio.h>

int main(){
	int N;	
	scanf("%d",&N);
	int i;
	for (i=0;i<N;i++)
		printf("%6d%c",i,((i%10)==9||i==N-1)?'\n':' ');
	return 0;
}
