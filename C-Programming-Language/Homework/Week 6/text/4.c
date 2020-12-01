#include <stdio.h>
#define MAXLEN 1000
int main()
{	char s[MAXLEN];
	char a[MAXLEN];
	int i=0;
	int j=0;
	int m=0;
	scanf("%s",s);
	while(s[i]!='\0')
		{if(s[i]=='a'||s[i]=='e'||s[i]=='i'||s[i]=='o'||s[i]=='u')
			a[j++]=s[i];
			i=i+1;}
		a[j]='\0';
		printf("%s",a);
		printf("\n");
	return 0;
	}
