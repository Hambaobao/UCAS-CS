#include <stdio.h>
#include <malloc.h>


#define MAXLINE 20

#define BUFFSIZE 100

char c;
int order1=0;
int order2=0;

int output[MAXLINE*3];
int outlen=0;

int insert=0;
int nodelen1=0,nodelen2=0;
int nodenumb1[MAXLINE];
int nodenumb2[MAXLINE];

typedef struct node{
	int data;
	int LTag,RTag;
	struct node * left;
	struct node * right;
}Node;

	Node * buffer[BUFFSIZE];
	int front=0;
	int rear=0;

Node *pre;	

void init_queue(void){
	front=0;
	rear=0;
}

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
			nodenumb1[nodelen1++]=a;	
		}
		else if(c=='n'){
			nodenumb1[nodelen1++]=0;   //½«¿ÕÊ÷ÓÃ0À´±êÊ¶£¬²»Ì«ºÃ£¬Èç¹û¶ÁÈëµÄÓÐ0ÄØ£¿
			getchar();
			getchar();
			getchar();
			c=getchar();
		}
	
	}while(c!=';');

	do{
		c=getchar();
		if(is_numb(c)){
			int a=0;
				do{
					a=a*10+c-'0';
				}while(is_numb(c=getchar()));
			nodenumb2[nodelen2++]=a;	
		}
		else if(c=='n'){
			nodenumb2[nodelen2++]=0;   //½«¿ÕÊ÷ÓÃ0À´±êÊ¶£¬²»Ì«ºÃ£¬Èç¹û¶ÁÈëµÄÓÐ0ÄØ£¿
			getchar();
			getchar();
			getchar();
			c=getchar();
		}
	
	}while(c!=';');

	c=getchar();
	if(is_numb(c)){
		int a=0;
			do{
				a=a*10+c-'0';
			}while(is_numb(c=getchar()));
		insert=a;	
	}	
}


void build_tree1(Node * tree){  //¹¹½¨treeµÄ×óÓÒº¢×Ó
	int data;

	data=nodenumb1[++order1];
	if(data!=0){
		Node *child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;
		child->data = data;
		child->LTag = child->RTag = 0; 
		tree->left=child;
		q_push(child);
	}

	data=nodenumb1[++order1];
	if(data!=0){
		Node * child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;  //ÕâÀïÒ»¶¨Òª×¢Òâ×óÓÒº¢×ÓµÄ³õÊ¼»¯
		child->data = data;
		child->LTag = child->RTag = 0; 
		tree->right=child;
		q_push(child);
	}
}

void build_tree2(Node * tree){  //¹¹½¨treeµÄ×óÓÒº¢×Ó
	int data;

	data=nodenumb2[++order2];
	if(data!=0){
		Node *child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;
		child->data = data;
		child->LTag = child->RTag = 0; 
		tree->left=child;
		q_push(child);
	}

	data=nodenumb2[++order2];
	if(data!=0){
		Node * child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;  //ÕâÀïÒ»¶¨Òª×¢Òâ×óÓÒº¢×ÓµÄ³õÊ¼»¯
		child->data = data;
		child->LTag = child->RTag = 0; 
		tree->right=child;
		q_push(child);
	}
}

int GetNextNode(Node *Tree){
	if(Tree->LTag)
		return Tree->data;
	else 
		return GetNextNode(Tree->left);
}

void visit(Node* Tree){
	if(Tree->data!=0){
		if(Tree->left && Tree->LTag==0)
			visit(Tree->left);

		output[outlen++]=Tree->LTag;
//		if(!Tree->LTag)
//			output[outlen++]=GetNextNode(Tree->left);
//		else
			output[outlen++]=Tree->left->data;

		output[outlen++]=Tree->RTag;
//		if(!Tree->RTag)
//			output[outlen++]=GetNextNode(Tree->right);
//		else
			output[outlen++]=Tree->right->data;

		if(Tree->right && Tree->RTag==0)
			visit(Tree->right);
	}
}


Node *findnode(Node *Tree,int data){
	Node *p;
	if(!Tree)
		return NULL;
	else if(Tree->data == data)
		return Tree;
	else if(Tree->left && (p = findnode(Tree->left,data)))
		return p;
	else if(Tree->right && (p = findnode(Tree->right,data)))
		return p;
}

void InThreading(Node *p){
	if(p)
	{
		InThreading(p->left);
		if(!p->left)
		{
			p->LTag = 1;
			p->left = pre;
		}
		if(!pre->right)
		{
			pre->RTag = 1;
			pre->right = p;
		}
		pre = p;
		InThreading(p->right);
	}
}

void InOderThreading(Node *Thrt,Node *T){
	if(!(Thrt = (Node *)malloc(sizeof(Node))))
		printf("overflow");
	Thrt->LTag = 0;
	Thrt->RTag = 1;
	Thrt->data = 0;
	if(!T)
		Thrt->left = Thrt;
	else
	{
		Thrt->left = T;
		pre = Thrt;
		InThreading(T);
		pre->right = Thrt;
		pre->RTag = 1;
		Thrt->right = pre;
	}
}

int main(){

	for(int i=0;i<MAXLINE;i++){
		nodenumb1[i]=0;
		nodenumb2[i]=0;
	}

	for(int i=0;i<MAXLINE*3;i++)
		output[i]=0;

	input();

	init_queue();

	Node *Tree1=(Node *)malloc(sizeof(Node));
	Tree1->data = nodenumb1[0];
	Tree1->left = Tree1->right = NULL;
	Tree1->LTag = Tree1->RTag =0;
	q_push(Tree1);

	do{
		build_tree1(q_pop());
	}while(!buff_empty());

	init_queue();

	Node * Tree2 = (Node *)malloc(sizeof(Node));
	Tree2->data = nodenumb2[0];
	Tree2->left = Tree2->right = NULL;
	Tree2->LTag = Tree2->RTag =0;	
	q_push(Tree2);

	do{
		build_tree2(q_pop());
	}while(!buff_empty());

	Node *insertpos;
	insertpos = findnode(Tree1,insert);

	if(!insertpos->left)
		insertpos->left=Tree2;
	else
	{
		Tree2->right=insertpos->left;
		insertpos->left=Tree2;
	}

	Node *Thrt;

	InOderThreading(Thrt,Tree1);

	visit(Tree1);

	for(int i=0;i<outlen-5;i++)
	{
		for(int j=0;j<3;j++)
			printf("%d,",output[i++]);
		printf("%d;",output[i]);
	}

	printf("%d,%d,%d,%d",output[outlen-4],output[outlen-3],output[outlen-2],output[outlen-1]);
	

	return 0;
}
