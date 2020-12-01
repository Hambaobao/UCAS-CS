#include <stdio.h>

#define MAXLEN 100

int main(){
	char A[MAXLEN],B[MAXLEN],C[MAXLEN],c;
	char D[MAXLEN],E[MAXLEN];
	int lena=0,lenb=0,lenc=0,lend=0,lene=0;
	
	do{
		scanf("%c",&A[lena++]);
	}while((c=getchar())!=';');

	do{
		scanf("%c",&B[lenb++]);
	}while((c=getchar())!=';');

	do{
		scanf("%c",&C[lenc++]);
	}while((c=getchar())!='\n');



	for(int i=0;i<lenb;i++){
		for(int j=0;j<lenc;j++)
			if(B[i]==C[j])
				E[lene++]=B[i];
	}

	for(int i=0;i<lena;i++){
		for(int j=0;j<lene;j++)
			if(A[i]==E[j])
				A[i]=' ';
	}

	for(int i=0;i<lena;i++)
		if(A[i]!=' ')
			D[lend++]=A[i];

	for(int i=0;i<lend-1;i++){
		printf("%c",D[i]);
		printf(",");
	}

	printf("%c",D[lend-1]);

	return 0;
}
