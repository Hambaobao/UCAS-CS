#include <stdio.h>
void squeeze(char [],int c);
int main(){
	char str[]="hello world!\n";
	int c='o';
	squeeze(str,c);
	printf("%s",str);
	return 0;
}

void squeeze(char s[],int c)
{	int i,j;
	for (i=j=0;s[i]!='\0';i++)
		if (s[i]!=c)
			s[j++]=s[i];
	s[j]='\0';	
}
