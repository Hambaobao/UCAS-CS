#include <stdio.h>
#include <stdlib.h>
struct lnode{
	int number;
	struct lnode *next;	
};

struct lnode* left(int n,int k,int t);
struct lnode* lalloc();
struct lnode * buildlist(int n);
int main(int argc,char *argv[]){
	int n,k,t;
	int count=1;
	struct lnode *leftlist,*pre,*p,*prepre;

	if(argc!=4 || (n=atoi(argv[1]))<=0 || (k=atoi(argv[2]))<=0 || (t=atoi(argv[3]))<=0){
		printf("Usage: ./game n k t\n");
		return 1;	
	}
	leftlist = left(n,k,t);

	pre=leftlist;
	prepre=leftlist;

	while(pre->next!=leftlist)
	{
		pre=pre->next;			
	}

	while(prepre->next!=pre)
	{
		prepre=prepre->next;
	}

	printf("left: ");
	while(t>0){
		if(count%k==0){
			printf("%d ",pre->number);
			pre=prepre->next;
			prepre->next=pre->next;
			free(pre);
			t--;		
		}else{
			prepre=prepre->next;
			pre=pre->next;
		}
		count++;	
	}

	printf("\n");	
	return 0;
}

struct lnode* left(int n,int k,int t){
	int count=1;
	struct lnode *plist,*pre,*p,*prepre;
	plist=buildlist(n);
	pre=plist;
	prepre=plist;

	while(pre->next!=plist)
	{
		pre=pre->next;			
	}

	while(prepre->next!=pre)
	{
		prepre=prepre->next;
	}

	while(n>t){
		if(count%k==0){
			pre=prepre->next;
			prepre->next=pre->next;
			free(pre);
			n--;		
		}else{
			prepre=prepre->next;
			pre=pre->next;
		}
		count++;	
	}

/*	
	while(n>t){
		if(count%k==0){
			p=pre->next;
			pre->next=p->next;
			free(p);
			n--;
		}else
			pre=pre->next;
		count++;
	}

*/
	return prepre;
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





