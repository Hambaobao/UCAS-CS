#include <stdio.h>
void func1();
int main(){
	printf("call func1 1st time\n");
	func1();
	printf("call func1 2nd time\n");
	func1();
	printf("call func1 3rd time\n");
	func1();
	return 0;
}

void func1(){
	int c=0;
	static int d=0;
	printf("	c=%d,d=%d\n",c,d);
	c++;
	d++;
}
