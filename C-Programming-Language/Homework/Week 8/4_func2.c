#include <stdio.h>

int externalvar;

void func2(){
	externalvar+=200;
	printf("1 in func2: externalvar=%d\n",externalvar);
}
