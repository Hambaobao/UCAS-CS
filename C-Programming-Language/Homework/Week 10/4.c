#include <stdio.h>

#define MAXLEN 100
#define forever for(;;)
#define max(A,B) ((A)>(B))?(A):(B)
#define square1(x) x*x
#define square2(x) (x)*(x)

int main(){
	int a=3,b=19;
	float c=9.0,d=8.3;
	printf("MAXLEN=%d\n",MAXLEN);
	printf("a=%3d,	b=%3d,	max(a,b)=%3d\n",a,b,max(a,b));
	printf("c=%3.1f,	d=%3.1f,	max(c,d)=%3.1f\n",c,d,max(c,d));
	printf("a=%3d,	b=%3d,	max(a++,b++)=%3d\n",a,b,max(a++,b++));
	printf("square1(%d+%d)=%d\n",a,b,square1(a+b));
	printf("square2(%d+%d)=%d\n",a,b,square2(a+b));
	return 0;	
}
