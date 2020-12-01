#include <stdio.h>
#include <malloc.h>


#define MAXLINE 100

#define BUFFSIZE 100

char inputstring[MAXLINE];

char nodenumb[MAXLINE];
int nodelen=0;
char c;
int order=0;

char output[MAXLINE];
int outlen=0;

int deepth[MAXLINE];

typedef struct node{
	char data;
	struct node * left;
	struct node * right;
}Node;

	Node * buffer[BUFFSIZE];
	int front=0;
	int rear=0;

void q_push(Node * a){
	if(rear<BUFFSIZE)
		buffer[rear++]=a;
}

Node * q_pop(void){
	if(front!=rear)
		return buffer[front++];
}

int buff_full(void){
	if(rear>=BUFFSIZE)
		return 1;
	else
		return 0;
}

int buff_empty(void){
	if(front==rear)
		return 1;
	else
		return 0;
}


int is_alp_numb(char c){
	if((c>='A' && c<='Z') || (c>='0' && c<='9'))
		return 1;
	else
		return 0;
}

int mygetline(char s[],int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i;
}

void input(void){
	int inputlen=mygetline(inputstring,MAXLINE);

	for(int i=0;i<inputlen;i++)
	{
		c=inputstring[i];
		if(is_alp_numb(c)){
			nodenumb[nodelen++]=c;		
		}
		else if(c=='n'){
			nodenumb[nodelen++]='@';
			i+=3;   
		}
	}
}

void build_tree(Node * tree){  //构建tree的左右孩子
	char data;

	data=nodenumb[++order];
	if(data!='@'){
		Node *child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;
		child->data = data; 
		tree->left=child;
		q_push(child);
	}

	data=nodenumb[++order];
	if(data!='@'){
		Node * child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;  //这里一定要注意左右孩子的初始化
		child->data = data; 
		tree->right=child;
		q_push(child);
	}
}

void visit(Node* Tree,int deep){
	if(Tree->data!='@')
	{
		output[outlen]=Tree->data;
		deepth[outlen]=deep;
		outlen++;

		if(Tree->left)
			visit(Tree->left,deep+1);
		if(Tree->right)
			visit(Tree->right,deep);
	}
}

int main(){

	for(int i=0;i<MAXLINE;i++){
		nodenumb[i]='@';
		deepth[i]=0;
	}


	input();

	Node *Tree=(Node *)malloc(sizeof(Node));
	Tree->data = nodenumb[0];
	Tree->left = Tree->right = NULL;
	q_push(Tree);

	do{
		build_tree(q_pop());
	}while(!buff_empty());


	visit(Tree,0);

	
	for(int i=0;i<outlen-1;i++)
	{
		while(deepth[i])
		{
			printf(" ");
			deepth[i]--;
		}
		printf("%c\n",output[i]);
	}

	while(deepth[outlen-1])
	{
		printf(" ");
		deepth[outlen-1]--;
	}

	printf("%c",output[outlen-1]);

/*
	for(int i=0;i<nodelen-1;i++)
		printf("%c ",nodenumb[i]);
	printf("%c",nodenumb[nodelen-1]);
	printf("\n");
*/
	
	return 0;
}