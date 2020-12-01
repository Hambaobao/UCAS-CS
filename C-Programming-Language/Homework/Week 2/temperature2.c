#include <stdio.h>
int main()
{	int f,c;
	int lower,upper,step;
	lower=0;
	upper=300;
	step=20;
	f=lower;
	while(f<=upper){
	c=5*(f-32)/9;
	printf("%d\t%d\n",f,c);
	f=f+step;
	}
	return 0;
}
