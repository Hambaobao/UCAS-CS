#include <stdio.h>
int main()
{	int nw;
	int c;
	nw=0;
	c=getchar();
	while(c!=EOF){
		++nw;
		c=getchar();}
	printf("\nthe total number:%d\n",nw);
	return 0;
}
