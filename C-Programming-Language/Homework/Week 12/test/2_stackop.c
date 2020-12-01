#include <stdio.h>
#define MAXVAL 100

int sp=0;
int error;
double val[MAXVAL];

void push(double f){
	if(sp<MAXVAL)
		val[sp++]=f;
	else
	{
		printf("error: stack full, can't push %g\n",f);
		error=1;
	}
}

double pop(void){
	if(sp>0)
		return val[--sp];
	else{
	//	printf("error: stack empty\n");
		error=1;
		return 0.0;
		}
}
