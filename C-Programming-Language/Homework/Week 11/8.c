#include <stdio.h>
int a[10]={0,1,2,3,4,5,6,7,8,9};
int main(){
	int *pa,i;
	pa=a;
	for(i=0;i<10;i++)
		printf("%d ",a[i]);
	printf("\n");

	for(i=0;i<10;i++)
		printf("%d ",*(a+i));
	printf("\n");

	for(i=0;i<10;i++)
		printf("%d ",pa[i]);
	printf("\n");

	for(i=0;i<10;i++)
		printf("%d ",*(pa+i));
	printf("\n");
	pa=&a[3];
	printf("%d\n",pa[2]);
	return 0;		
}
