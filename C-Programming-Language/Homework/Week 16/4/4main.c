#include <stdio.h>
#include "4geo.h"

int main(){
	struct rect screen;
	struct point middle,a1,a2,a3;
	struct point makepoint(double,double);
	screen.pt1=makepoint((double)3,(double)27);
	screen.pt2=makepoint((double)18,(double)7);
	printf("screen:\n");
	printf("pt1(%.2lf,%.2lf) pt2(%.2lf,%.2lf)\n",screen.pt1.x,screen.pt1.y,screen.pt2.x,screen.pt2.y);
	screen=canonrect(screen);
	printf("pt1(%.2lf,%.2lf) pt2(%.2lf,%.2lf)\n",screen.pt1.x,screen.pt1.y,screen.pt2.x,screen.pt2.y);
	middle=middlepoint(screen);
	printf("middle:(%.2lf,%.2lf)\n",middle.x,middle.y);
	a1=makepoint(4,8);
	printf("a1(%.2lf,%.2lf)",a1.x,a1.y);
	printf("is %s in screen\n",ptinrect(a1,screen)?"":"not");
	a2=makepoint(11,2);
	printf("a2(%.2lf,%.2lf)",a2.x,a2.y);
	printf("is %s in screen\n",ptinrect(a2,screen)?"":"not");
	a3=addpoint(a1,a2);
	printf("a3(%.2lf,%.2lf)\n",a3.x,a3.y);
	return 0;
}
