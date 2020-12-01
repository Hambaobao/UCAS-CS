#include <stdio.h>
#include <string.h>

#define MAXLEN 10

void reverse(char t[]);
int main(){
	char s[]="hello everyone";
	reverse(&s[4]);
	printf("%s",s);
	printf("\n");
	return 0;
}

void reverse(char *t){
	int c,i,j;
	for(i=0,j=strlen(t)-1;i<j;i++,j--)
		c=t[i],t[i]=t[j],t[j]=c;	
}
