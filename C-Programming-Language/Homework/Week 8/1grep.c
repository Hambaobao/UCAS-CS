#include <stdio.h>
#define MAXLINE	 100

char pattern []="ould";

int mygetline(char [],int );

int strindex(char [],char []);

int main(){
	char line[MAXLINE];
	int found=0;
	while (mygetline(line,MAXLINE)>=0)
		if (strindex(line,pattern)>=0){
			printf("%s",line);
			found++;
		}
	return found;	
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

int strindex(char s[],char t[]){
	int i=0;
	int j,k;
	for(i=0;s[i]!='\0';i++)
		{
			{
				for(j=i,k=0;t[k]!='\0'&&s[j]==t[k];j++,k++)
						;				
							}
			if(k>0 && t[k]=='\0')
				return i;
		}
	return -1;			
}
