#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 200

typedef struct vex{
	int data;
	struct vex *next;
}Vex;

int s[MAXSIZE];
int reached[MAXSIZE];


int dfs(int u,int v,int nodenumb,Vex *A[nodenumb+1])
{
	if(u == v)
			return 1;

	Vex *p = A[u];
	for(;p != NULL;p = p -> next)
	{
		if(!reached[p -> data])
		{
			reached[p -> data] = 1;
			int r = dfs(p -> data,v,nodenumb,A);
			reached[p -> data] = 0;
			if(r)
					return 1;
		}
	}
	return 0;
}

int cycle(int nodenumb,Vex *A[nodenumb+1]){

	for(int i=1;i<=nodenumb;i++)
	{
		Vex *p=A[i];

		while(p)
		{
			int r=dfs(p->data,i,nodenumb,A);

			if(r)
				return 1;

			p=p->next;			
		}
	}

	return 0;
}

int main(){
	char c;
	int nodenumb;
	scanf("%d",&nodenumb);
	getchar();

	int fst[nodenumb+1];
	int lst[nodenumb+1];

	Vex *A[nodenumb+1];

	for(int i=0;i<MAXSIZE;i++)
		s[i]=0;

	for(int i=0;i<MAXSIZE;i++)
		reached[i]=0;

	for(int i=0;i<nodenumb+1;i++)
	{
		fst[i]=0;
		lst[i]=0;
	}

	for(int i=0;i<nodenumb+1;i++)
		A[i]=NULL;

	int slen=0;
	do{
		scanf("%d",&s[slen++]);
		c=getchar();
	}while(c!='\n');

	int fslen=0;
	do{
		scanf("%d",&fst[fslen++]);
		c=getchar();
	}while(c!='\n');

	int lslen=0;
	do{
		scanf("%d",&lst[lslen++]);
		c=getchar();
	}while(c!='\n');

	for(int i=1;i<=nodenumb;i++)
	{
		int from=fst[i];
		int to=lst[i];

		if(from<=to)
		{
			for(int k=from;k<=to;k++)
			{
				if(s[k] && s[k]!=i)
				{
					Vex *ve,*p;
					ve=(Vex *)malloc(sizeof(Vex));
					ve->data=s[k];
					ve->next=NULL;

					if(!A[i])
						A[i]=ve;
					else
					{
						p=A[i];
						while(p->next && p->data!=ve->data)
							p=p->next;

						if(p->data!=ve->data)	
							p->next=ve;
					}
				}
			}
		}
	}


	if(cycle(nodenumb,A))
		printf("yes");
	else
		printf("no");

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