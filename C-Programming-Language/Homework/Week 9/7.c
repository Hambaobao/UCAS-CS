#include <stdio.h>
int foo(int x);
int c=0;
int main(){
	int result=6;
	result=foo(result);
	printf("c=%d,result=%d\n",c,result);
	result=foo(result);
	printf("c=%d,result=%d\n",c,result);
	return 0;
}

int foo(int x){
	int a;
	static int b=3;
	a=2*x;
	c+=(++a,b++);
	return a+b+c;
}
