#include <stdio.h>
#include <stdlib.h>

struct lnode{
	int data;
	struct lnode *next;
};

int main(){
	struct lnode *plist,*p,*q,*r,*s;
	char c;
	int min,max;
	p=(struct lnode *)malloc(sizeof(struct lnode));
	plist=p;

	do{
		q=(struct lnode *)malloc(sizeof(struct lnode));
		scanf("%d",&(q->data));
		p->next=q;
		p=q;
	}while((c=getchar())!='\n');
	q->next=NULL;
	
	max=q->data;

	p=plist->next;

	while(p->next!=q){
		p=p->next;
	}
	
	min=p->data;

	r=plist->next;
	while(r->next!=p)
		r=r->next;
	r->next=NULL;

	p=plist;
	while(((p->next)->data)<=min)
		p=p->next;

	q=p->next;
	while((q->data)<=max && q->next!=NULL)
		q=q->next;

	r=p->next;

	while(r!=q){
		s=r->next;
		free(r);
		r=s;
	}

	p->next=q;

	if(q->data<max){
		p->next=NULL;
	}	

//	printf("%d %d",min,max);

	p=plist->next;
	while(p->next!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}

	printf("%d",p->data);

//	printf("\n");
	return 0;
}

