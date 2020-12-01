#include <stdio.h>

#define MAXLEN 1000


int main(){
	int va[MAXLEN],len=0;
	int x,cnt=0;
	char c;

	do{
		scanf("%d",&va[len]);
		len++;	
	}while((c=getchar())!='\n');

	x=va[len-1];	

	while(x>va[cnt]){
		cnt++;
	}

	for(int i=len-1;i>cnt;i--)
		va[i]=va[i-1];

	va[cnt]=x;


	for(int i=0;i<len-1;i++)
		printf("%d ",va[i]);
	printf("%d",va[len-1]);

	return 0;	
}



















