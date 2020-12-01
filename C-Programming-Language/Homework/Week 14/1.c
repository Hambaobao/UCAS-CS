#include <stdio.h>

int func1(int a,int b);
int func2(int a,int b);

int main(){
	int choice;
	int a=10,b=8,c;
	int (*pf)(int ,int );
	printf("input your choice\n");
	printf("1 for func1, 2 for func2 :\n");
	scanf("%d",&choice);
	if(choice==1)
		pf=func1;
	else if(choice==2)
		pf=func2;
	else
	{
		printf("please input 1 or 2\n");
		return 1;
	}
	c=(*pf)(a,b);
	printf("a=%d b=%d result=%d\n",a,b,c);
	return 0;
}

int func1(int a,int b)
{
	return a+b;
}

int func2(int a,int b)
{
	return a-b;	
}
