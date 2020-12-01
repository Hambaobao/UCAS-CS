#include <stdio.h>

#define MAXLINE	100

int find(int L[MAXLINE],int R[MAXLINE],int u,int v){
	if(L[v]==u || R[v]==u)
		return 1;
	else if(L[v]==0 && R[v]==0)
		return 0;
	else
		return (find(L,R,u,L[v]) || find(L,R,u,R[v]));
}


int main(){
	int L[MAXLINE];
	int R[MAXLINE];
	char c;
	int Llen=0;
	int Rlen=0;

	do{
		scanf("%d",&L[Llen++]);
		c=getchar();
	}while(c!=';');

	do{
		scanf("%d",&R[Rlen++]);
		c=getchar();
	}while(c!=';');

	int u,v;
	scanf("%d",&u);
	getchar();
	scanf("%d",&v);


	printf("%d",find(L,R,u,v));



/*
	for(int i=0;i<Llen;i++)
		printf("%d ",L[i]);
	printf("\n");

	for(int i=0;i<Llen;i++)
		printf("%d ",R[i]);
	printf("\n");

	printf("%d %d\n",u,v);	
*/

	return 0;
}
