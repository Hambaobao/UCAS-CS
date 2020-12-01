#include "3myhead.h"

int main(){
	struct node *pnode;
	char keyword[100];
	buildlist();
	printf("Please input keyword:\n");
	scanf("%s",(char *)keyword);
	pnode=(struct node *)lookup(keyword);
	if(pnode){
		printf("%s>>>%s\n",pnode->name,pnode->defn);
	}else{
		printf("not found\n");
	}
	return 0;
}
