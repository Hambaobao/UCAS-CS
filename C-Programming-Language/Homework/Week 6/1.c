#include <stdio.h>
int main(){
	int a,b,c,d;
	a=b=c=d=0;
	if (a++>1||b++<1||c++==1);
		d=a+b+c++;
	printf("a=%d b=%d c=%d d=%d\n",a,b,c,d);
	
	if (a++>3||++b<2||c++)
		d=++a+b+c--;
	printf("a=%d b=%d c=%d d=%d\n",a,b,c,d);

	if (a++>=2&&++b>=1&&c++>0)
		d=a+b+c++;
	printf("a=%d b=%d c=%d d=%d\n",a,b,c,d);
	return 0;
}
