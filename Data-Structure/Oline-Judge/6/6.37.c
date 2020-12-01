#include <stdio.h>
#include <malloc.h>

//#include <iostream>
//#include <stdlib.h>

//using namespace std;

#define MAXLINE 20

#define BUFFSIZE 100

int nodenumb[MAXLINE];
int nodelen=0;
char c;
int order=0;

int output[MAXLINE];
int outlen=0;

typedef struct node{
	int data;
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


int is_numb(char c){
	if(c>='0' && c<='9')
		return 1;
	else
		return 0;
}

void input(void){
	do{
		c=getchar();

		if(is_numb(c)){
			int a=0;
				do{
					a=a*10+c-'0';
				}while(is_numb(c=getchar()));
			nodenumb[nodelen++]=a;	
		}
		else if(c=='n'){
			nodenumb[nodelen++]=0;
			getchar();
			getchar();
			getchar();
			c=getchar();
		}

	
	}while(c!='\n' && !buff_full());
}

void add_node(Node *tree){
	int data=nodenumb[order++];
	if(data==0){
		tree->left=NULL;
		tree->right=NULL;
		return ;
	}
	else{
		tree->data=data;
		tree->left=(Node *)malloc(sizeof(Node));
		tree->right=(Node *)malloc(sizeof(Node));
		q_push(tree->left);
		q_push(tree->right);
	}
}

void visit(Node* Tree){
	if(Tree==NULL)
		return ;
	else
	{
		output[outlen++]=Tree->data;
		if(Tree->left!=NULL)
			visit(Tree->left);
		if(Tree->right!=NULL)
			visit(Tree->right);
	}
}


int main(){

	for(int i=0;i<MAXLINE;i++)
		nodenumb[i]=0;

	input();

	Node *Tree=(Node *)malloc(sizeof(Node));

	q_push(Tree);

	do{
		add_node(q_pop());
	}while(!buff_empty());


	visit(Tree);

//	printf("\n");

/*
	for(int i=0;i<nodelen;i++)
		printf("%d ",nodenumb[i]);
	printf("\n");
*/

	for(int i=0;i<outlen-1;i++)
		printf("%d,",output[i]);
	printf("%d",output[outlen-1]);

	return 0;
}