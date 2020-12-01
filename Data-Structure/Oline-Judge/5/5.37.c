#include<stdio.h>
 
#include<malloc.h>
 
#define MAXNUM 100 // 输入串的最大长度
char s[MAXNUM];//记录输入串
 
int index = 0;
int level = 0;//记录当前层数
int maxLevel = 0;//记录最大层数
 

typedef struct node 
{    
    int tag;    
    union
    {
    	int num;
        char data;
        struct node* head;
    };
    struct node* next;
} Node;

 
Node* setList();  //递归创建一个表
 
Node*  setElement()  //用于递归过程中判断当前需要创建原子结点还是表结点
{
	int isnumb(char var);

    Node* h = NULL;
    if(s[index] == '(')//发现 ’(’ 说明是一个子表
    {
        h = setList(); // 返回一个子表
    }
    else if(s[index] == ')') //只有遇见类似’ ( ) ’的结构时才会走到这一步，使表结点的head指向NULL,（对照setList()函数）
    {
        index++;
        h = NULL;
 
    }
    else if(isnumb(s[index]))//数字原子结点
    {
    	int a=0;
    	while(isnumb(s[index])){
    		a=a*10+s[index]-'0';
    		index++;
    	}

        h = (Node*)malloc(sizeof(Node));
        h -> tag = 2;
        h -> num = a;
    }
    else//原子结点
    {
        h = (Node*)malloc(sizeof(Node));
        h -> tag = 0;
        h -> data = s[index];
        index++;
    }
 
    return h;
 
}
 
Node* setList()
{
    Node*p,*h = NULL;
    h = (Node*)malloc(sizeof(Node));//分配表结点
    h -> tag = 1;
    h -> next = NULL;
    index++;
 
 
    h -> head = setElement();//head指向表中的第一个元素
    p = h -> head;
 
    if(p) // 防止’( ) ’出现使p为NULL
    {
        while(s[index] == ',')  // 与head同一层表元素的便利，依次连接到head后
        {
            index++;
            p -> next = setElement(); // 原子结点的创建或子表的递归
            p = p -> next;
        }
        p -> next = NULL;
 
        if(s[index] != '\0')
            index++;
    }
 
 
    return h; 
}
 

void show(Node* List) 
{
    if(!List)
    {
        return ;
    }

    if(maxLevel < level)
    {
        maxLevel = level;
    }

 
    Node* travel = List;
    while(travel)
    {
        if(travel -> tag == 0)
        {
            printf("%c",travel -> data);
        }
        else if(travel -> tag == 2){
        	printf("%d",travel -> num);
        }
        else
        {
 
            printf("(");
            level++;  
            show(travel -> head);
            level--;   
            printf(")");
        }
        travel = travel -> next;
    }
 
}

int isnumb(char var){
	if(var>='0'&&var<='9')
		return 1;
	return 0;
}

int main(){

	int a;
	char b;
	char c;

	scanf("%d",&a);
	c=getchar();

	while((b=getchar())!='\n' && index<MAXNUM)
		s[index++]=b;
	s[index]='\0';

    Node* ans = setList();
    show(ans);

	return 0;
}


