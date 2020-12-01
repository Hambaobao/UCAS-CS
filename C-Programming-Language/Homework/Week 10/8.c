#include <stdio.h>

int main(){
	int a=10;
	int b=3;
#if !defined(HDR)
	a+=b;
#endif
	a*=b;
	printf("result=%d\n",a);
	return 0;	
}
