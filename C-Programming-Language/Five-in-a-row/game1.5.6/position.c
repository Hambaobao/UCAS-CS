#include "head.h"

struct point AI(void){
	struct point zl;
	int pmax=0;
	int distance=0;
	int dmin=225;

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable[i][j].score=get_grades_black(i,j);


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax<gradestable[i][j].score)
				pmax=gradestable[i][j].score;


	int l=0;

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==gradestable[i][j].score)
			{
				BlackGradesMax[l].x=i;
				BlackGradesMax[l].y=j;
				l++;
			}

					//选出最靠近中心的点
	for(int l=0;l<16;l++)
		if((distance = (BlackGradesMax[l].x-8)*(BlackGradesMax[l].x-8)+(BlackGradesMax[l].y-8)*(BlackGradesMax[l].y-8)) < dmin)		//到中心的距离	
			dmin=distance;

	for(int l=0;l<10;l++)
		if((distance = (BlackGradesMax[l].x-8)*(BlackGradesMax[l].x-8)+(BlackGradesMax[l].y-8)*(BlackGradesMax[l].y-8)) == dmin)
			{
				bestpblack[l].x=BlackGradesMax[l].x;
				bestpblack[l].y=BlackGradesMax[l].y;
			}
			

	
	for(int i=0;i<10;i++)
		if(bestpblack[i].x!=0)
		{
			zl.x=bestpblack[i].x;
			zl.y=bestpblack[i].y;		
		break;
		}

	return zl;
		
}








