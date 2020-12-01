#include <stdio.h>
int main()
{	int c,f;
	f=0;
	while(f<=300){
	c=5*(f-32)/9;
	printf("%d\t%d\n",f,c);
	f=f+20;
	}
	return 0;
}
