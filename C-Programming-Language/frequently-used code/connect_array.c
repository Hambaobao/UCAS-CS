#include <stdio.h>
#define MAXLEN 100
int main()
{	void mystrcat(char [],char []);
	char s[MAXLEN]="hello ";
	char t[]="world!";
	mystrcat(s,t);
	printf("s=%s\n",s);
	return 0;
}
void mystrcat(char s[],char t[])
{	int i,j;
	i=j=0;
	while(s[i]!='\0')
		i++;
	while(t[j]!='\0')
		{s[i++]=t[j];
		j++;}
	s[i]=t[j];
}
