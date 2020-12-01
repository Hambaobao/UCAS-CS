#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20

typedef struct vex{
	int data;
	struct vex *next;
}Vex;

Vex *A[MAXSIZE];

int s[MAXSIZE];

int dfs(int u,int v,int step)
{
	if(step == 0 && u == v)
			return 1;
	Vex *p = A[u];
	for(;p != NULL;p = p -> next)
	{
		if(!s[p -> data])
		{
			s[p -> data] = 1;
			int r = dfs(p -> data,v,step - 1);
			s[p -> data] = 0;
			if(r)
					return 1;
		}
	}
	return 0;
}

int GetMPL(int v,int mpl)
{
	Vex *p=A[v];

	if(!p)
		return mpl;

	int temp1=mpl;

	while(p)
	{
		int temp = GetMPL(p->data,mpl+1);

		if(temp>temp1)
			temp1=temp;

		p=p->next;			
	}

	return temp1;			
}

int main(){
	int nodenumb;
	int v,w,res;
	char c;

	for(int i=0;i<MAXSIZE;i++)
		{
			A[i]=NULL;
			s[i]=0;
		}

	scanf("%d",&nodenumb);
	getchar();

	int MPL[nodenumb+1];

	do{
		scanf("%d",&v);
		getchar();
		scanf("%d",&w);

		Vex *ve,*p;
		ve=(Vex *)malloc(sizeof(Vex));

		ve->data=w;
		ve->next=NULL;

		if(!A[v])
			A[v]=ve;
		else
		{
			p=A[v];
			while(p->next)
				p=p->next;

			p->next=ve;
		}

	}while((c=getchar())!='\n');


	for(int i=1;i<=nodenumb;i++)
	{
		MPL[i]=GetMPL(i,0);
	}

	for(int i=1;i<nodenumb;i++)
		printf("%d,",MPL[i]);
	printf("%d",MPL[nodenumb]);

/*
	printf("\n");
	for(int i=1;i<nodenumb+1;i++)
	{
		Vex *p=A[i];

		printf("node%d:",i);
		while(p){
			printf("%d ",p->data);
			p=p->next;
		}
		printf("\n");
	}
*/


	return 0;
}