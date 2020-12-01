#include <stdio.h>
#define MAXLINE	 100

char pattern []="ould";

int mygetline(char [],int );

int strindex(char [],char []);

int main(){
	char line[MAXLINE];
	int d,m,n;
	d=strindex(line,pattern);
	if(d>=3)
		{	
		m=d-4;
		n=m+3;
		printf("the rightest ould is from %d to %d\n",m,n);
							}
	else
		printf("there is no ould in s");
	
	return 0;	
}

int strindex(char s[],char t[]){
	int i,b;
	int j,k;
	b=mygetline(s,MAXLINE);
	for(i=b;i>=3;i--)
		{
			{
				for(j=i,k=3;k>=0&&s[j]==t[k];j--,k--)
						;				
							}
			if(k=-1)
				{
					return i;break;
								}
		}
	return -1;			
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
