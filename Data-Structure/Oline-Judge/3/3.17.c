#include <stdio.h>

#define MAXLEN 100

int main(){
	int judge(char s[MAXLEN],int len);
	char s[MAXLEN],c;
	int len=0;
	do{
		scanf("%c",&s[len++]);
	}while(s[len-1]!='@');
	
	printf("%d",judge(s,len-1));
	return 0;
}

int judge(char s[MAXLEN],int len){
	int i,j;
	if((len/2)==0)
		return 0;
	
	for(i=0;i<len;i++)
		if(s[i]!='&')
			;
		else if(s[i]=='&')
			break;

	for(j=i;i>=0;i--,j++){
		if(s[i]==s[j])
			;
		else
			return 0;
	}
	return 1;
}
