#include <stdio.h>
#include <malloc.h>


#define MAXLINE 20

#define BUFFSIZE 100

#define PLUS	-1
#define SUB		-2
#define MULT	-3
#define DIV		-4				

int nodenumb[MAXLINE];
int nodelen=0;
char c;
int order=0;

int output[MAXLINE];
int outlen=0;

int deepth=0;
int bef=0;

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

void init_queue(void){
	front=0;
	rear=0;
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
		else if(c=='+'){
			nodenumb[nodelen++]=-1;
			c=getchar();
		}
		else if(c=='-'){
			nodenumb[nodelen++]=-2;
			c=getchar();
		}
		else if(c=='*'){
			nodenumb[nodelen++]=-3;
			c=getchar();
		}
		else if(c=='/'){
			nodenumb[nodelen++]=-4;
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
	deepth++;
	if(Tree->data!=0){
		if(Tree->data==PLUS){
			if(deepth==1){
				visit(Tree->left);
				printf("+");
				bef=PLUS;	
				visit(Tree->right);				
			}
			else
			{
				if(bef!=PLUS){	
					printf("(");
					visit(Tree->left);
					printf("+");
					bef=PLUS;
					visit(Tree->right);
					printf(")");
				}
				else
				{
					visit(Tree->left);
					printf("+");
					bef=PLUS;
					visit(Tree->right);					
				}
			}
		}
		else if(Tree->data==SUB){
			if(deepth==1){
				visit(Tree->left);
				printf("-");
				bef=SUB;
				visit(Tree->right);				
			}
			else
			{
				if(bef!=PLUS){	
					printf("(");
					visit(Tree->left);
					printf("-");
					bef=SUB;
					visit(Tree->right);
					printf(")");
				}
				else
				{
					visit(Tree->left);
					printf("-");
					bef=SUB;
					visit(Tree->right);					
				}
			}
			bef=SUB;
		}
		else if(Tree->data==MULT){
			bef=MULT;
			visit(Tree->left);
			printf("*");
			bef=MULT;
			visit(Tree->right);			
		}
		else if(Tree->data==DIV){
			bef=DIV;
			visit(Tree->left);
			printf("/");
			bef=DIV;
			visit(Tree->right);			
		}
		else 
			printf("%d",Tree->data);
	}
}


int main(){

	for(int i=0;i<MAXLINE;i++)
		nodenumb[i]=0;

		input();

	Node *Tree=(Node *)malloc(sizeof(Node));
	Tree->data = nodenumb[0];
	init_queue();
	q_push(Tree);

	do{
		build_tree(q_pop());
	}while(!buff_empty());
	
	visit(Tree);

	return 0;
}
