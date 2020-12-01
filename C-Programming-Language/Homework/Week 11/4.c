#include <stdio.h>
int main(){
	int x=1,y=5,i=0;
	int z[]={101,102,103};
	int *ip1;
	int *ip2;
	printf("%d x=%d y=%d z[0]=%d z[1]=%d z[2]=%d\n",i++,x,y,z[0],z[1],z[2]);
	ip1=&x;
	y=*ip1;
	printf("%d x=%d y=%d z[0]=%d z[1]=%d z[2]=%d\n",i++,x,y,z[0],z[1],z[2]);
	*ip1=0;
	printf("%d x=%d y=%d z[0]=%d z[1]=%d z[2]=%d\n",i++,x,y,z[0],z[1],z[2]);
	ip1=&z[0];
	*ip1=8;
	printf("%d x=%d y=%d z[0]=%d z[1]=%d z[2]=%d\n",i++,x,y,z[0],z[1],z[2]);
	*(ip1+1)=10;
	ip2=ip1+2;
	printf("%d x=%d y=%d z[0]=%d z[1]=%d z[2]=%d\n",i++,x,y,z[0],z[1],z[2]);
	*ip2=55;
	printf("%d x=%d y=%d z[0]=%d z[1]=%d z[2]=%d\n",i++,x,y,z[0],z[1],z[2]);
	return 0;
}
