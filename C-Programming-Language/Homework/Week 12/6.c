#include <stdio.h>

int main(){
	int array[]={0,1,2,3,4,5,6,7,8,9};
	int *p;
	p=&array[5];
	printf("*p=%d \n",*p);
	printf("*++p=%d \n",*++p);
	printf("*p=%d \n",*p);
	printf("*--p=%d \n",*--p);
	printf("*p=%d \n",*p);
	printf("*p++=%d \n",*p++);
	printf("*p=%d \n",*p);
	printf("*p--=%d \n",*p--);
	printf("*p=%d \n",*p);
	return 0;
}
