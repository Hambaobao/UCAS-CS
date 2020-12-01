#include <stdio.h>
#include <string.h>
#define MAXLEN 100

int mygetline(char s[],int lim);
void move(char input[],int m,int n);
int main(){
	int m,n;
	char input[MAXLEN];
	n=mygetline(input,MAXLEN);
	//printf("%d\n",n);
	scanf("%d",&m);
	move(input,m,n);
	printf("%s",input);
	return 0;
}

void move(char input[],int m,int n){
	char temp[n];
	for(int i=m,k=0;i<n;i++,k++)
		temp[k]=input[i];
	for(int i=0,k=n-m;i<n;i++,k++)
		temp[k]=input[i];
	for(int i=0;i<n;i++)
		input[i]=temp[i];	
}

int mygetline(char s[],int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i-1;
}


