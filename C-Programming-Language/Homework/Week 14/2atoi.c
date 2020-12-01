#include <stdio.h>
#define MAXLEN 100

char s[MAXLEN];

int main(){
	void itoa1(int ,char []);
	void itoa2(int ,char []);
	int n;
	scanf("%d",&n);
	if(n<0)
	{
		s[0]='-';
		n=(-1*n);
		itoa1(n,s);
	}
	else
	{
		itoa2(n,s);
	}
	printf("%s\n",s);
	return 0;
}

void itoa1(int n,char s[])
{
	static int i=1;	
	if(n/10)
		itoa1(n/10,s);
	s[i++]=(n%10+'0');
}

void itoa2(int n,char s[])
{	
	static int j=0;
	if(n/10)
		itoa2(n/10,s);
	s[j++]=(n%10+'0');
}

