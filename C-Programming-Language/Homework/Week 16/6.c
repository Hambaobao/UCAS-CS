#include <stdio.h>

struct point{
	double x;
	double y;
};

struct rect{
	struct point pt1;
	struct point pt2;
};

int main(){
	struct point a={3,9},b={5,12};
	struct point *pp;
	struct rect r,*pr;
	printf("a:(%.2lf,%.2lf)\n",a.x,a.y);
	pp=&a;
	pp->x+=10;
	pp->y-=4;
	printf("*pp:(%.2lf,%.2lf)\n",(*pp).x,(*pp).y);
	printf("*pp:(%.2lf,%.2lf)\n",pp->x,pp->y);
	pr=&r;
	pr->pt1=a;
	pr->pt2=b;
	printf("r:(%.2lf,%.2lf) (%.2lf,%.2lf)\n",r.pt1.x,r.pt1.y,r.pt2.x,r.pt2.y);
	return 0;
}
