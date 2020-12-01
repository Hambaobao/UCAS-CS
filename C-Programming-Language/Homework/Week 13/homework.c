#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define WIDTH 10
#define DEEP 10000
#define MAXLINE 100

void assign(int s[DEEP][WIDTH],int i);
int Is_same(int s[DEEP][WIDTH],int i);
void odd_add(int s[DEEP][WIDTH],int i);
int a_to_i (char s[]);

int main(int argc,char *argv[]){
	int i=0;	
	int s[DEEP][WIDTH];
	char temp1[MAXLINE];
	char temp2[WIDTH][MAXLINE];

	while(argc != 11)
	{
		printf("Invalid inputs!---Usage: 10 12 13 14 15 16 1 3 14 7\n");
		printf("please restart and input again\n");
		return 0;
	}

	for(int i=1;i<11;i++)
	for(int t=0;('0'<=(*(argv+i))[t])&&((*(argv+i))[t]<='9');t++)
		temp2[i-1][t]=(*(argv+i))[t];


	for(int i=0;i<11;i++)
	{
		for(int q=0;q<MAXLINE;q++)
			temp1[q]=temp2[i][q];
		s[0][i]=a_to_i(temp1);
	}

/*	
	printf("please input the amount of candies of each kid\n");		//命令行逐次输入
	for(int j=1;j<=10;j++)
	{
		printf("kid%d: ",j);scanf("%d",&s[0][j-1]);
	}
				*/
	while(!Is_same(s,i))
		{
			odd_add(s,i++);
			assign(s,i++);
		}

	for(int k=0;k<i;k++)
		{
			for(int j=0;j<WIDTH;j++)
				printf("%d\t",s[k][j]);
			printf("\n");	
		}
								
	return 0;
}

void assign(int s[DEEP][WIDTH],int i){
	for(int j=1;j<WIDTH;j++)
		s[i+1][j]=(s[i][j]/2+s[i][j-1]/2);
	s[i+1][0]=(s[i][0]/2+s[i][WIDTH-1]/2);			
}

int Is_same(int s[DEEP][WIDTH],int i){
	int n=0;
	for(int j=1;(j<WIDTH) && (s[i][j-1]==s[i][j]);j++)
			n++;
	if(n==WIDTH-1)
		return 1;
	else
		return 0;
}

void odd_add(int s[DEEP][WIDTH],int i){
	for(int j=0;j<WIDTH;j++)
		if((s[i][j]%2)!=0)
			s[i+1][j]=s[i][j]+1;
		else
			s[i+1][j]=s[i][j];
}


int a_to_i (char s[]){
	int i,n,sign;
	for(i=0;isspace(s[i]);i++)
		;		//跳过空白符;
	sign=(s[i]=='-')?-1:1;
	if(s[i]=='+'||s[i]=='-')//跳过符号
  		i++;
	for(n=0;isdigit(s[i]);i++)
	       n=10*n+(s[i]-'0');//将数字字符转换成整形数字
	return sign *n;
}





