#include <stdio.h>
#include <string.h>

#define MAXLEN 10

void reverse(char s[]);
int main(){
	char s[]="Hello everyone";
	reverse (s);
	printf("%s\n",s);
	return 0;
}

void reverse(char s[]){
	int c,i,j;
	for (i=0,j=strlen(s)-1;i<j;i++,j--){
		c=s[i];
		s[i]=s[j];
		s[j]=c;
	}
}
