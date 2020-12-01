#include <stdio.h>
#define LEN 5

void print2D(int a[][LEN]);
int main(){
	int a[LEN][LEN]={
		{0,1,2,3,4},
		{10,11,12,13,14},
		{20,21,22,23,24},
		{30,31,32,33,34},
		{40,41,42,43,44},	
	};
	printf("original data in a[%d][%d]\n",LEN,LEN);
	print2D(a);
	a[0][2]=100;
	a[1][4]=200;
	a[0][5]=300;
	a[0][10]=400;
	printf("new data in a[%d][%d]\n",LEN,LEN);
	print2D(a);
	return 0;
}

void print2D(int a[][LEN])
{
	int i,j;
	for(i=0;i<LEN;i++)
	{
		for(j=0;j<LEN;j++)
			printf("%3d, ",a[i][j]);
		printf("\n");
	}
}
