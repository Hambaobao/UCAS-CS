#include <stdio.h>
#include <math.h>

struct point{
	double x;
	double y;
};

int main(){
	double distance(struct point p1,struct point p2);
	struct point a={1,2},b={4,6};
	printf("a=(%.lf,%.lf) b=(%.lf,%.lf)\n",a.x,a.y,b.x,b.y);
	printf("distance=%.lf\n",distance(a,b));
	return 0;
}

double distance(struct point p1,struct point p2)
{
	return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
