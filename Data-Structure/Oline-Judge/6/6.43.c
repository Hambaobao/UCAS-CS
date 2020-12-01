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
			nodenumb[nodelen++]=0;   //½«¿ÕÊ÷ÓÃ0À´±êÊ¶£¬²»Ì«ºÃ£¬Èç¹û¶ÁÈëµÄÓÐ0ÄØ£¿
			getchar();
			getchar();
			getchar();
			c=getchar();
		}

	
	}while(c!='\n' && !buff_full());
}

void build_tree(Node * tree){  //¹¹½¨treeµÄ×óÓÒº¢×Ó
	int data;

	data=nodenumb[++order];
	if(data!=0){
		Node *child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;
		child->data = data; 
		tree->left=child;
		q_push(child);
	}

	data=nodenumb[++order];
	if(data!=0){
		Node * child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;  //ÕâÀïÒ»¶¨Òª×¢Òâ×óÓÒº¢×ÓµÄ³õÊ¼»¯
		child->data = data; 
		tree->right=child;
		q_push(child);
	}
}

void visit(Node* Tree){
	if(Tree->data!=0){
		output[outlen++]=Tree->data;
		if(Tree->right)
			visit(Tree->right);
		if(Tree->left)
			visit(Tree->left);
	}
}

int main(){

	for(int i=0;i<MAXLINE;i++)
		nodenumb[i]=0;

		input();
	//cout<<nodenumb[0]<<nodenumb[1]<<nodenumb[5]<<nodenumb[6]<<endl;
	Node *Tree=(Node *)malloc(sizeof(Node));
	Tree->left = Tree->right =NULL;
	Tree->data = nodenumb[0];
	q_push(Tree);

	do{
		build_tree(q_pop());
	}while(!buff_empty());


	visit(Tree);

	
	//cout<<outlen<<endl;
	for(int i=0;i<outlen-1;i++)
		printf("%d,",output[i]);
	printf("%d",output[outlen-1]);
	


	return 0;
}
