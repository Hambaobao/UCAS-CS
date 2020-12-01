#include <stdio.h>
char carray[]="helloworld!";
int iarray[]={0,1,2,3,4,5,6,7,8,9,10};
double darray[]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};

int main(){
	char *pc=carray;
	int *pi=iarray;
	double *pd=darray,*ppd;
	printf("before ++ pc=%p pi=%p pd=%p \n",pc,pi,pd);
	pc++;pi++;pd++;
	printf("after ++ pc=%p pi=%p pd=%p \n",pc,pi,pd);
	pc+=5;pi+=5;pd+=5;
	printf("after +=5 pc=%p pi=%p pd=%p \n",pc,pi,pd);
	ppd=&darray[5];
	if(ppd>pd)
		printf("ppd>pd\n");
	else
		printf("ppd<pd\n");
	return 0;
}
