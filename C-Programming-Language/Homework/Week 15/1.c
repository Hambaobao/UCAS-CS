#include <stdio.h>
int main(){
	int a=10,i;
	int arr[4]={4,5,6,7};
	int *pa=&a;
	int *ap[4]={&arr[0],&arr[1],&arr[2],&arr[3]};
	int **pp;
	pp=&pa;
	printf("a:%d,&a:%p,&pa:%p,pa:%p,*pa:%d\n",a,&a,&pa,pa,*pa);
	printf("pp:%p,*pp:%p,**pp:%d\n",pp,*pp,**pp);
	pp=&ap[0];
	for(i=0;i<4;i++)
		printf("%3d",**pp++);
	printf("\n");
	return 0;
}
