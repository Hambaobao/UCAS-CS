#include <stdio.h>
int main(){
	float a=0,b=0;
	float sign=1.0,demo=1.0;
	while((1.0/demo)>=0.000001||(1.0/demo)<=-0.000001){
		a=a+sign*(1.0/demo);
		b=4.0*a;
		sign=sign*(-1.0);
		demo=demo+2.0;}
	printf("%f\n",b);
	return 0;
}
