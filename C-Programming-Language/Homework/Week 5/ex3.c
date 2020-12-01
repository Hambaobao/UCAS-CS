#include <stdio.h>
#define MAXLENGTH 1000
int mystrlen(char s[]);
int main(){
	char mystring[MAXLENGTH];
	int len;
	scanf("%s",mystring);
	len=mystrlen(mystring);
	printf("Len=%d\n",len);
	return 0;
}
int mystrlen(char s[]){
	int i;
	i=0;
	while(s[i]!='\0')
		++i;
	return i;
}
