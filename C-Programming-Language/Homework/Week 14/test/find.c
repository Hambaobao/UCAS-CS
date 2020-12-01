#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int mygetline(char*line,int max);
int samenum(char *,char *);

int main(int argc,char *argv[]){
	char line[MAXLINE];
	long lineno=0;
	int c,except=0,number=0,found=0;
	int n=0,i;
	while(--argc>0 && (*++argv)[0]=='-')
		while(c=*++argv[0])
			switch(c)
			{
				case '0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
					n=10*n+c-'0';break;
				case 'x':
					except = 1; break;
				case 'n':
					number = 1; break;
				default:
					printf("find: illegal option%c\n",c);
					argc=0; found= -1; break;
			}
	if(argc!=1||n==0)
		printf("Usage: find -number -x -n pattern\n");
	else
		while(mygetline(line,MAXLINE)>0)
		{
			lineno++;
			if(samenum(line,*argv)>=n && except==0)
			{
				if(number)
					printf("%ld:",lineno);
				printf("%s",line);
				found++;
			}
			else if(samenum(line,*argv)<n && except==1)
			{
				if(number)
					printf("%ld:",lineno);
				printf("%s",line);
				found++;
			}
		}
	return found;			
}

int mygetline(char *s,int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i;
} 

int samenum(char *s,char *arg)
{
	char *pl;
	int l,n=0;
	pl=s;

	while(strstr(pl,arg)!=NULL)
	{
		pl=strstr(pl,arg)+strlen(arg);
		n++;	
	}
	return n;	
}


