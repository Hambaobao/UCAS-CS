#include <stdio.h>
int mystrlen (char *s);
int main(){
	char c[]="hello friends!";
	int len;
	len=mystrlen(c);
	printf("len=%d\n",len);
	return 0;
}

int mystrlen(char *s){
	char *p=s;
	while (*p!='\0')
		p++;
	return p-s;
}
