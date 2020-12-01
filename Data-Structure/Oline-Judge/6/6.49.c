#include <stdio.h>
#include <malloc.h>


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
		if(Tree->left)
			visit(Tree->left);
		if(Tree->right)
			visit(Tree->right);
	}
}

int CompBTNode(Node * b){
//	BTNode *Qu[MaxSize],*p; //定义一个队列，用于层次遍历
	
//	int front=0,rear=0; //队头、队尾指针
	init_queue();

	int cm=1;
	int bj=1;

	Node *p;

	if(b!=NULL)
	{
		q_push(b); //进队

		while(!buff_empty()){

			p=q_pop();

			if(p->left==NULL) //*p节点没有左孩子
			{
				bj=0;
				if(p->right!=NULL) //没有左孩子但有右孩子
					cm=0; //则不是完全二叉树
			}
			else //*p节点有左子树
			{
				if(bj==1) //迄今为止，所有节点均有左右孩子
				{
					q_push(p->left); //左孩子进队

					if(p->right==NULL) //*p有左孩子但没有右孩子
						bj=0;
					else
					{
						q_push(p->right); //右孩子进队
					}
				}		
				else //bj=0:迄今为止，已有节点缺孩子
					cm=0; //而此时*p节点有左孩子，违反（2）
			}
		}
	return cm;
	}

	return 1; //把空树当成特殊的完全二叉树

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
	
	printf("%d",CompBTNode(Tree));

	return 0;
}
