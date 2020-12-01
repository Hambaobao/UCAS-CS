#include <stdio.h>
int main(){
	float sum,term;
	float sign,deno,upper;
	sum=0;	
	sign=1;
	deno=1;
	upper=100;
	while(deno<=upper){
		term = sign * (1/deno);
		sum = sum + term;
		sign = -1 * sign;
		deno = deno + 1;
	}
	printf("sum = %f\n",sum);
	return 0;
}
