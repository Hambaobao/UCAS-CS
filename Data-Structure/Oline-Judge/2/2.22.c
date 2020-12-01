#include <stdio.h>

#define MAXLEN 1000

int main(){
	char s[MAXLEN];
	int len=0;
	char c;

	do{
		scanf("%c",&s[len]);
		len++;	
	}while((c=getchar())!='\n');
	
	for(int i=len-1;i>0;i--){
		printf("%c",s[i]);
		printf(",");	
	}
	printf("%c",s[0]);
	return 0;
}
