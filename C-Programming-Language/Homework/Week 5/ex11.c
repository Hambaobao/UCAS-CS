#include <stdio.h>
int main(){
	int a=5,b=10,sum=0;
	sum=sum + ++a;
	printf("a=%d b=%d sum=%d\n",a,b,sum);
	sum=sum + --b + a--;
	printf("a=%d b=%d sum=%d\n",a,b,sum);
	sum=sum + --b + a--;
	printf("a=%d b=%d sum=%d\n",a,b,sum);
	sum=sum + --b + a++;
	printf("a=%d b=%d sum=%d\n",a,b,sum);
	return 0;
}
