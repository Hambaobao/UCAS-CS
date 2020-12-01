#include <stdio.h>
int main(){
	unsigned getbits(unsigned, int, int );
	int b,a=0x56674;
	b=getbits(a,5,2);
	printf("b=%x\n",b);
	return 0;
}
unsigned getbits(unsigned x, int p, int n){
	return(x>>(p+1-n)&~(~0<<n));
}
