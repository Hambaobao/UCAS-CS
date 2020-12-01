#include <stdio.h>

#define N 5

int main(){
	int i;
	for ( i=1; i<N; i++)
		printf("you have %d item%s. \n", i, (i==1)?"":"s");
	return 0;
}
