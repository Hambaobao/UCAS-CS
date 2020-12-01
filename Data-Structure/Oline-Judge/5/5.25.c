#include <stdio.h>

int main(){
	int row,col;

	scanf("%d",&row);
	getchar();
	scanf("%d",&col);
	getchar();

	int M[row][col];
	int B[row][col];
	int A[row*col];

	char c;

	getchar();

	for(int i=0;i<row;i++){
		getchar();
		for(int j=0;j<col;j++){
			scanf("%d",&M[i][j]);
			c=getchar();
		}
		c=getchar();
	}	

	getchar();

	int k=0;
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++){
			if(M[i][j]){
				A[k++]=M[i][j];
				B[i][j]=1;
			}else
				B[i][j]=0;
		}

	printf("[");

	if(k>0){
		for(int i=0;i<k-1;i++)
			printf("%d,",A[i]);

		printf("%d",A[k-1]);
	}
	printf("]");

	printf(";");

	printf("[");

	for(int i=0;i<row-1;i++){
		printf("[");
		for(int j=0;j<col-1;j++)
			printf("%d,",B[i][j]);
		printf("%d",B[i][col-1]);
		printf("]");
		printf(";");
	}

		printf("[");
		for(int j=0;j<col-1;j++)
			printf("%d,",B[row-1][j]);
		printf("%d",B[row-1][col-1]);
		printf("]");

	printf("]");	



/*
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++)
			printf("%d ",M[i][j]);
		printf("\n");
	}
*/
	return 0;
}