#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20

typedef struct vex{
	int data;
	struct vex *next;
}Vex;

Vex *A[MAXSIZE];

int s[MAXSIZE];
//int step=0;

/*int PATH(int v,int w,int k){
	if(k<0)
		return 0;

	Vex *p=A[v];
	s[v]=1;

	if(p->data == w)
		return 1;
	
	int r=0;

	while(p=p->next)
	{
		if(!s[p->data])
		{
			s[p->data] = 1;
			r=PATH(p->data,w,k--);
			s[p->data] = 0;
			if(r)
				return 1;

		}			
	}
	return 0;
}*/

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

int main(){
	int nodenumb,k;
	int v,w,m,n,res;
	char c;

	for(int i=0;i<MAXSIZE;i++)
		{
			A[i]=NULL;
			s[i]=0;
		}

	scanf("%d,%d",&nodenumb,&k);
	getchar();

	scanf("%d,%d",&m,&n);
	getchar();

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

	s[m] = 1;
	res=dfs(m,n,k);

	if(res)
		printf("yes");
	else
		printf("no");

	//printf("step=%d\n",step);

	return 0;
}