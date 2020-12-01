#include "4geo.h"

struct point middlepoint(struct rect screen){
	struct point temp;
	temp=makepoint((screen.pt1.x+screen.pt2.x)/2,(screen.pt1.y+screen.pt2.y)/2);
	return temp;
}
