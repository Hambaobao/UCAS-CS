#ifndef _GEO_H
#define _GEO_H
#define XMAX 100
struct point{
	double x;
	double y;
};
struct rect{
	struct point pt1;
	struct point pt2;
};
struct point makepoint(double x,double y);
struct point middlepoint(struct rect screen);
struct point addpoint(struct point p1,struct point p2);
int pinrect(struct point p, struct rect r);
struct rect canonrect(struct rect r);
#endif 
