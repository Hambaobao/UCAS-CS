#include <stdio.h>
int main(){
	int x=1,y=2;
	int z[]={3,4};
	int *ip;
	printf("	%14s	%14s	%14s	%14s\n","x","y","z[0]","z[1]");
	printf("data:%14d	%14d	%14d	%14d\n",x,y,z[0],z[1]);
	printf("addr:%14p	%14p	%14p	%14p\n",&x,&y,&z[0],&z[1]);
	ip=&x;
	printf("ip=%p\n",ip);
	y=*ip;
	printf("y=%d\n",y);
	*ip=0;
	printf("x=%d\n",x);
	ip=&z[0];
	printf("ip=%p\n",ip);
	*ip=8;
	printf("z[0]=%d\n",z[0]);
	*(ip+1)=9;	
	printf("z[1]=%d\n",z[1]);
	return 0;
}
