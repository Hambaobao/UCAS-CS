#include "1myhead.h"

/*
struct tnode * addtree(struct tnode * ptree,char *word){
	struct tnode *p;
	int cmpresult;
	if(ptree==NULL){
		p= talloc();
		p->word=mystrdup(word);
		p->count=1;
		p->left=p->right=NULL;
		return p;
	}
	if((cmpresult=strcmp(word,ptree->word))==0){
		ptree->count++;
	}else if(cmpresult<0){
		ptree->left=addtree(ptree->left,word);
	}else{
		ptree->right=addtree(ptree->right,word);
	}
	return ptree;
}
*/


struct tnode *addtree(struct tnode *ptree,char *word){
	struct tnode *p,*q;
	int cmpresult;
	if(ptree==NULL){
		p= talloc();
		p->word=mystrdup(word);
		p->count=1;
		p->left=p->right=NULL;
		return p;
	}else{
		q=ptree;
		while((((cmpresult=strcmp(word,q->word))<0)&&(q->left!=NULL))||((cmpresult>0)&&(q->right!=NULL))){
			if((cmpresult<0)&&(q->left!=NULL)){
				q=q->left;
			}else{
				q=q->right;
			}
		}
		if(cmpresult==0){
			q->count++;
		}else if(cmpresult<0){
			p= talloc();
			p->word=mystrdup(word);
			p->count=1;
			p->left=p->right=NULL;
			q->left=p;		
		}else{
			p= talloc();
			p->word=mystrdup(word);
			p->count=1;
			p->left=p->right=NULL;
			q->right=p;			
		}
	}
	return ptree;
}


struct tnode *talloc(void){
	return (struct tnode*)malloc(sizeof(struct tnode));
}

char *mystrdup(char *s){
	char *p;
	p=(char*)malloc(strlen(s)+1);
	if(p!=NULL)
		strcpy(p,s);
	return p;
}

void treeprint(struct tnode * ptree){
	if(ptree){
		treeprint(ptree->left);
		printf("%4d %s\n",ptree->count,ptree->word);
		treeprint(ptree->right);
	}
}

int getword(char *word,int lim){
	int c,getch(void);
	void ungetch(int);
	char *w=word;
	while(isspace(c=getch()))
			;
	if (c!=EOF)
		{
			*w++=c;
		}
	if(!isalpha(c)){
		*w='\0';
		return c;
	}
	for(;--lim>0;w++)
		if(!isalnum(*w=getch())){
			ungetch(*w);
			break;
		}
	*w='\0';
	return w[0];		
}


#define BUFSIZE 100

char buf[BUFSIZE];//用于ungetch函数缓冲区
int bufp=0;       //buf中的下一个空闲位置
int getch(void){  //取一个字符（可能是压回的字符）
	return (bufp>0)?buf[--bufp]:getchar();
}

void ungetch(int c){//把字符压回到输入中
	if(bufp>=BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++]=c;
}
