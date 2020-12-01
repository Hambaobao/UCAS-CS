#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20

typedef struct vex{
	int data;
	struct vex *next;
}Vex;

Vex *A[MAXSIZE];

int s[MAXSIZE];

int PATH(int v,int w){
	Vex *p=A[v];

	s[v]=1;
	int r=0;

	while(p)
	{
		if(!s[p->data])
		{
			if(p->data == w)
				return 1;

			r=PATH(p->data,w);
			if(r)
				return 1;
		}	
				p=p->next;			
	}

	return 0;
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

	scanf("%d,%d",&v,&w);

	res=PATH(v,w);

	if(res)
		printf("yes");
	else
		printf("no");

	return 0;
}