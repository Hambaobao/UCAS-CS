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
	struct rect screen={
		{1,3},
		{4,5},
	};
	printf("point1:(%.lf,%.lf) point2:(%.lf,%.lf)\n",screen.pt1.x,screen.pt1.y,screen.pt2.x,screen.pt2.y);
	printf("squre:%.lf\n",(screen.pt2.x-screen.pt1.x)*(screen.pt2.y-screen.pt1.y));
	return 0;
}
