#include <stdio.h>
int foo(int x);
int main(){
	int a=5,b=1,sum=0;
	sum+=foo(a);
	printf("sum=%d\n",sum);
	sum+=foo(b);
	printf("sum=%d\n",sum);
	return 0;
}

int foo(int x){
	static int temp=2;
	switch(x){
		case 1:
			temp+=x+10;
		case 3:
			temp-=3;
		case 5:
			temp*=x;
	}
	return temp;
}
