#include <stdio.h>

int externalvar=100;

void func1();

void func2();

int main(){
	externalvar-=10;
	printf("1 in main: externalvar=%d\n",externalvar);
	func1();
	printf("2 in main: externalvar=%d\n",externalvar);
	func2();
	printf("3 in main: externalvar=%d\n",externalvar);
	return 0;
}

void func1(){
	externalvar+=100;
	printf("1 in func1: externalvar=%d\n",externalvar);
}

