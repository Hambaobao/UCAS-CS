#include<stdio.h>
#define MAXLEN 100

int atoi(char []);

int main(){
	char s[MAXLEN];
	int data;
	scanf("%s",s);
	data=atoi(s);
	printf("%d\n",data);
	return 0;
}

int atoi(char s[]){
	int i,n;
	n=0;
	for(i=0;s[i]>='0'&&s[i]<='9';++i)
		n=10*n+(s[i]-'0');
	return n;
}
