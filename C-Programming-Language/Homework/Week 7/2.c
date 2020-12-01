#include <stdio.h>

int main(){
	int a,num;
	num=0;
	scanf("%d",&a);
	switch (a){
		case 0: num=num+0;
		case 1: num=num+1;
		case 2: num=num+2;
		case 3: num=num+3;
		case 4: num=num+4;
		case 5: num=num+5;
		case 6: num=num+6;
	}
	printf("num=%d\n",num);
	return 0;
}
