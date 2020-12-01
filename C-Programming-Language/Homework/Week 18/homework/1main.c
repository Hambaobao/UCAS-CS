#include "1myhead.h"
int main()
{	struct tnode *ptree;
	char word[MAXWORD];
	ptree=NULL;
	while(getword(word,MAXWORD)!=EOF){
		if(isalpha(word[0])){
			ptree=addtree(ptree,word);
		}

	}
	printf("the results are:\n");
	treeprint(ptree);
	return 0;
}
