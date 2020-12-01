#include <stdio.h>
#include <malloc.h>

#define MAXLINE 100
#define MAXNODE 15
/*
typedef struct edge
{
	int tail;
	Edge *next=NULL;
}Edge;
*/
int main(){
	int node_numb;
	scanf("%d",&node_numb);
	getchar();
	int edge_numb;
	scanf("%d",&edge_numb);
	getchar();


	int real[MAXNODE+1][MAXNODE+1];

	for(int i=0;i<MAXNODE+1;i++)
		for(int j=0;j<MAXNODE+1;j++)
			real[i][j]=0;

	int effc[MAXNODE+1];
	for(int i=0;i<MAXNODE+1;i++)
		effc[i]=0;	

	char c;
	do{
		int head,rear;
		scanf("%d",&head);
		getchar();
		scanf("%d",&rear);
		c=getchar();

		real[head][rear]=1;
		effc[head]=1;
		effc[rear]=1;
	}while(c!='\n');

/*
	for(int i=1;i<MAXNODE+1;i++){
		for(int j=1;j<edge_numb+1;j++)
			printf("%d ",real[i][j]);
		printf("\n");
	}

	for(int i=1;i<MAXNODE+1;i++)
		printf("%d ",effc[i]);
	printf("\n");
*/


	int sum[MAXNODE+1];
	for(int i=0;i<MAXNODE+1;i++)
		sum[i]=0;

	for(int i=0;i<MAXNODE+1;i++)
		for(int j=0;j<MAXNODE+1;j++)
			sum[i]+=real[i][j];
/*
	for(int i=1;i<MAXNODE+1;i++)
		printf("%d ",sum[i]);
	printf("\n");
*/

	for(int i=0;i<MAXNODE+1;i++){
		int j;
		if(effc[i]&&sum[i]>0){
			printf("%d ",i);
			for(j=MAXNODE;sum[i]>1;j--){
				if(real[i][j]){
					printf("%d,",j);
					sum[i]--;
				}
			}
			for(;sum[i]>0;j--){
				if(real[i][j]){
					printf("%d\n",j);
					sum[i]--;
				}
			}
		}else if(effc[i]&&sum[i]==0){
			printf("%d\n",i);
		}
	}

	return 0;
}