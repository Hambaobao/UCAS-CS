#include <stdio.h>
#define MAXLEN 100
int main(){
	void mystrcpy(char *s,char *t);
	char a[MAXLEN];
	char b[]="I am a string";
	mystrcpy(a,b);
	printf("%s\n",a);
	return 0;
}

void mystrcpy(char *s, char *t){
	while((*s++=*t++));	
}
