#include <stdio.h>
#include <malloc.h>

#define MAXLINE 20

#define BUFFSIZE 100

char c;
int order=0;

int output[MAXLINE];
int outlen=0;

typedef struct node{
	int data;
	struct node * left;
	struct node * right;
}Node;

int pre[MAXLINE];
int mid[MAXLINE];
int prelen=0;
int midlen=0;


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
			pre[prelen++]=a;	
		}
		else if(c=='n'){
			pre[prelen++]=0;   //½«¿ÕÊ÷ÓÃ0À´±êÊ¶£¬²»Ì«ºÃ£¬Èç¹û¶ÁÈëµÄÓÐ0ÄØ£¿
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
			mid[midlen++]=a;	
		}
		else if(c=='n'){
			mid[midlen++]=0;   //½«¿ÕÊ÷ÓÃ0À´±êÊ¶£¬²»Ì«ºÃ£¬Èç¹û¶ÁÈëµÄÓÐ0ÄØ£¿
			getchar();
			getchar();
			getchar();
			c=getchar();
		}
	
	}while(c!='\n');
}

/*
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
*/

Node *build_tree(Node *tree,int root,int mid[MAXLINE],int midlen){

	tree->data=root;
	tree->left=tree->right=NULL;

	int leftnode[midlen];
	int rightnode[midlen];

	int midl=0,leftlen,rightlen;
	for(leftlen=0;mid[midl]!=root;leftlen++,midl++)
		leftnode[leftlen]=mid[midl];
	midl++;
	for(rightlen=0;midl<midlen;rightlen++,midl++)
		rightnode[rightlen]=mid[midl];

	if(leftlen>0)
	{
		Node *child=(Node *)malloc(sizeof(Node));
		child->left=child->right=NULL;
	 	tree->left=build_tree(child,pre[++order],leftnode,leftlen);
	}

	if(rightlen>0)
	{
		Node *child=(Node *)malloc(sizeof(Node));
		child->left=child->right=NULL;
	 	tree->right=build_tree(child,pre[++order],rightnode,rightlen);		
	}

	return tree;

}

void visit(Node* Tree){
	if(Tree->data!=0){
		if(Tree->left)
			visit(Tree->left);
		if(Tree->right)
			visit(Tree->right);
		output[outlen++]=Tree->data;
	}
}

int main(){

	for(int i=0;i<MAXLINE;i++){
		pre[i]=0;
		mid[i]=0;
	}

	input();
/*
	for(int i=0;i<prelen;i++)
		printf("%d ",pre[i]);
	printf("\n");

	for(int i=0;i<midlen;i++)
		printf("%d ",mid[i]);
	printf("\n");
*/
	Node *Tree=(Node *)malloc(sizeof(Node));
	build_tree(Tree,pre[order],mid,midlen);

	visit(Tree);
	
	for(int i=0;i<outlen-1;i++)
		printf("%d,",output[i]);
	printf("%d",output[outlen-1]);
	


	return 0;
}
