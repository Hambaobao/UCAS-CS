#include <stdio.h>

#define MAXLEN 100

int mygetline(char s[],int lim);
int judge(char s[MAXLEN],int len);

int main(){
	int n=0,len;

	char s[MAXLEN],c;
	
	do{
		len=mygetline(s,MAXLEN);
		printf("%d\n",judge(s,len));
	}while(c=getchar()!=EOF);

	return 0;
}



int mygetline(char s[],int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i;
}

int judge(char s[MAXLEN],int len){
	int i=0;
	int n=0;
	while(s[i]!='('){
		i++;
	}
	while(i<=len){
		if(s[i]=='(')
			n++;
		else if(s[i]==')')
			n--;
		i++;	
	}
	if(n==0)
		return 1;
	else 
		return 0;	
}
