#include <stdio.h>
#define MAXSIZE 50

int Get_Saddle(int A[MAXSIZE][MAXSIZE],int m,int n,int Res[MAXSIZE]);

int main()
{
	int r_n = 0;
	int c_n = 0;
	int array[MAXSIZE*10];
	int i = 0;
	char c;
	do{
		scanf("%d",&array[i++]);
		c = getchar();
		if(c == ';')
			r_n++;
	}while(c != '\n');

	r_n++;

	c_n = i/r_n;

	int Matrix[MAXSIZE][MAXSIZE];
	int Res[MAXSIZE];
	int find;

	i=0;
	for(int j=0;j<r_n;j++)
		for(int k=0;k<c_n;k++)
			Matrix[j][k] = array[i++];

	find = Get_Saddle(Matrix,r_n,c_n,Res);

	if(find == 0)
		printf("null");
	else if(find == 1)
			printf("%d",Res[0]);
	else{
		for(int i=0;i<find-1;i++)
			printf("%d,",Res[i]);
		printf("%d",Res[find-1]);
	}
/*
	for(int i=0;i<r_n;i++){
		for(int j=0;j<c_n;j++)
			printf("%d ",Matrix[i][j]);
		printf("\n");
	} 
*/
	return 0;
}

int Get_Saddle(int A[MAXSIZE][MAXSIZE],int m,int n,int Res[MAXSIZE])
{
	int i,j,flag,min,k;
	int numb=0;
	for(i=0;i<m;i++)
	{
		min=A[i][0];
		for(j=0;j<n;j++)
			if(A[i][j]<min)
				min=A[i][j];

			for(j=0;j<n;j++){
				if(A[i][j]==min)   
				{    flag=1;
					 for(k=0;k<m;k++)
						 if(min<A[k][j]) flag=0;
					 if(flag){
						 Res[numb++]=A[i][j];
					 }
				}
			}
      
	   }
	return numb;
 
}
