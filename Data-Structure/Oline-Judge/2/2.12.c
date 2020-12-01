#include <stdio.h>

#define MAXLEN 1000

int main(){
	int lena=0,lenb=0,len,i=0;	
	char A[MAXLEN],B[MAXLEN],c;

	do{
		scanf("%c",&A[lena]);
		lena++;
	}while((c=getchar())!=';');
	
	do{
		scanf("%c",&B[lenb]);
		lenb++;
	}while((c=getchar())!='\n');

	len=(lena>lenb)?lena:lenb;

	while((A[i])==B[i])
		i++;

	if(i==len)
		printf("%d",0);
	else if(A[i]<B[i])
		printf("%d",1);
	else if(A[i]>B[i])
		printf("%d",2);
	
	printf("\n");
	return 0;
}	

