#include <stdio.h>

int main(){
	double a;
	double x,y,z;
	double myabs(double ,double );
	scanf("%lf",&a);
	if(a<0)
		printf("error: a<0\n");
	else{
		x=a;
		do{
			y=0.5*(x+a/x);
			z=myabs(x,y);		
			x=y;		
				}while(z>=0.00001);
		printf("a^(1/2) = %lf\n",x);
						}
	return 0;
}

double myabs(double x,double y){
	if(x>=y)
		return x-y;
	else
		return y-x;
}
