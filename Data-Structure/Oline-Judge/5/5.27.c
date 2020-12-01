#include <stdio.h>

int main(){
	int row,col;

	scanf("%d",&row);
	getchar();
	scanf("%d",&col);
	getchar();

	int A[row][col];
	int B[row][col];

	char c;

	getchar();
		
	for(int i=0;i<row;i++){
		getchar();
		for(int j=0;j<col;j++){
			scanf("%d",&A[i][j]);
			c=getchar();
		}
		c=getchar();
	}

	getchar();

	getchar();
		
	for(int i=0;i<row;i++){
		getchar();
		for(int j=0;j<col;j++){
			scanf("%d",&B[i][j]);
			c=getchar();
		}
		c=getchar();
	}

	getchar();

/*
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++)
			printf("%d ",A[i][j]);
		printf("\n");
	}

	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++)
			printf("%d ",B[i][j]);
		printf("\n");
	}
*/

	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			A[i][j]+=B[i][j];

	printf("[");

	for(int i=0;i<row-1;i++){
		printf("[");
		for(int j=0;j<col-1;j++)
			printf("%d,",A[i][j]);
		printf("%d",A[i][col-1]);
		printf("]");
		printf(";");
	}

		printf("[");
		for(int j=0;j<col-1;j++)
			printf("%d,",A[row-1][j]);
		printf("%d",A[row-1][col-1]);
		printf("]");

	printf("]");		


	return 0;
}	