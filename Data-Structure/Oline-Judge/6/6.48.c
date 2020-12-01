#include <stdio.h>
#include <malloc.h>


#define MAXLINE 100

#define BUFFSIZE 500

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

Node *node1,*node2;

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


void build_tree(Node * tree){  //¹¹½¨treeµÄ×óÓÒº¢×Ó
	int data;

	data=nodenumb[++order];
	if(data!=-1){
		Node *child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;
		child->data = data; 
		tree->left=child;
		q_push(child);
	}

	data=nodenumb[++order];
	if(data!=-1){
		Node * child = (Node *)malloc(sizeof(Node));
		child->left = child->right = NULL;  //ÕâÀïÒ»¶¨Òª×¢Òâ×óÓÒº¢×ÓµÄ³õÊ¼»¯
		child->data = data; 
		tree->right=child;
		q_push(child);
	}
}

Node *visit(Node* Tree,int q){
	Node *p;
	if(Tree->data==q){
		p=Tree;
		return p;
	}
	else{	
		if(Tree->right&&(p=visit(Tree->right,q)))
			return p;
		if(Tree->left&&(p=visit(Tree->left,q)))
			return p;
	}	
}


int findnode(Node *Tree,Node *q){
	int flag=0;
	if(!Tree)
		return 0;
	if(Tree->left==q || Tree->right==q || Tree==q)
		return 1;
	else
		return (findnode(Tree->left,q) || findnode(Tree->right,q));

}


Node *find(Node *Tree,Node *node1,Node *node2){
	if(findnode(Tree->left,node1) && findnode(Tree->left,node2))
		return find(Tree->left,node1,node2);
	else if(findnode(Tree->right,node1) && findnode(Tree->right,node2))
		return find(Tree->right,node1,node2);
	return Tree;
}

int main(){

	for(int i=0;i<MAXLINE;i++)
		nodenumb[i]=-1;

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
			nodenumb[nodelen++]=-1;   //½«¿ÕÊ÷ÓÃ0À´±êÊ¶£¬²»Ì«ºÃ£¬Èç¹û¶ÁÈëµÄÓÐ0ÄØ£¿
			getchar();
			getchar();
			getchar();
			c=getchar();
		}
	
	}while(c!=';' && !buff_full());

	int a,b;

	scanf("%d",&a);
	getchar();
	scanf("%d",&b);
	
	Node *Tree=(Node *)malloc(sizeof(Node));
	Tree->left = Tree->right =NULL;
	Tree->data = nodenumb[0];
	q_push(Tree);

	do{
		build_tree(q_pop());
	}while(!buff_empty());

	node1=visit(Tree,a);
	node2=visit(Tree,b);

	printf("%d",find(Tree,node1,node2)->data);

	return 0;
}
