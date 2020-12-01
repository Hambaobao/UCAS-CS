#include <stdio.h>
#include <stdlib.h>
struct lnode{
	int number;
	struct lnode *next;	
};

int left(int n,int k);
struct lnode* lalloc();
struct lnode * buildlist(int n);
int main(int argc,char *argv[]){
	int n,k,leftno;
	if(argc!=3 || (n=atoi(argv[1]))<=0 || (k=atoi(argv[2]))<=0){
		printf("Usage: ./game n k \n");
		return 1;	
	}
	leftno = left(n,k);
	printf("left:%d\n",leftno);
	return 0;
}

int left(int n,int k){
	int count=1,leftno;
	struct lnode *plist,*pre,*p;
	plist=buildlist(n);
	pre=plist;
	while(pre->next!=plist)
		pre=pre->next;
	while(n>1){
		if(count%k==0){
			p=pre->next;
			pre->next=p->next;
			free(p);
			n--;
		}else
			pre=pre->next;
		count++;
	}
	leftno=pre->number;
	free(pre);
	return leftno;
}

struct lnode *buildlist(int n){
	struct lnode *plist,*plast,*p;
	plist=lalloc();
	plist->number=n;
	plist->next=plist;
	plast=plist;

	while(--n>0){
		p=lalloc();
		p->number=n;
		p->next=plist;
		plist=p;
	}
	plast->next=plist;
	return plist;
}

struct lnode* lalloc(void){
	return (struct lnode*)malloc(sizeof(struct lnode));
}





