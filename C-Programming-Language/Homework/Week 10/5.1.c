#include <stdio.h>

#define square1(x) x*x
#define square2(x) (x)*(x)
#define dprint(expr) printf("expr=%g\n",expr)
int main(){
	double a=3.0,b=19.0;
	dprint(square1(a+b));
	dprint(square2(a+b));
	return 0;	
}
