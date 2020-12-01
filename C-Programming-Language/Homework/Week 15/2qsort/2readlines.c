#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 1000

int mygetline(char *,int );
char *alloc(int );

int readlines(char *lineptr[],int maxlines){
	int len,nlines;
	char *p,line[MAXLEN];
	nlines=0;
	while((len=mygetline(line,MAXLEN))>0)
		if(nlines>=maxlines || (p=alloc(len))==NULL)
			return -1;
		else{
			line[len-1]='\0';
			strcpy(p,line);
			lineptr[nlines++]=p;			
		}
	return nlines;

}

void writelines(char *lineptr[],int nlines){
	int i;
	printf("after sorting:\n");
	for(i=0;i<nlines;i++)
		printf("%s\n",lineptr[i]);
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
