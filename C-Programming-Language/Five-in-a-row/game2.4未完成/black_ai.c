#include "head.h"
#include  <stdlib.h>

struct point AI_Black(void){

	struct point zl;
	int pmax=0;

	int distance=0;
	int dmin=225;


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_black[i][j].score=get_grades_black(i,j);

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax<gradestable_black[i][j].score)
				pmax=gradestable_black[i][j].score;

	for(int i=0;i<250;i++)
		{
			BlackGradesMax[i].x=0;
			BlackGradesMax[i].y=0;	
		}

	for(int i=0;i<250;i++)
		{
			bestpblack[i].x=0;
			bestpblack[i].y=0;	
		}



	int l=0;									//选出最靠近中心的棋子

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==gradestable_black[i][j].score)
			{
				BlackGradesMax[l].x=i;
				BlackGradesMax[l].y=j;
				l++;
			}


	for(int l=0;l<250;l++)
		if((distance = (BlackGradesMax[l].x-8)*(BlackGradesMax[l].x-8)+(BlackGradesMax[l].y-8)*(BlackGradesMax[l].y-8)) < dmin)		//到中心的距离	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (BlackGradesMax[l].x-8)*(BlackGradesMax[l].x-8)+(BlackGradesMax[l].y-8)*(BlackGradesMax[l].y-8)) == dmin)
			{
				bestpblack[l].x=BlackGradesMax[l].x;
				bestpblack[l].y=BlackGradesMax[l].y;
			}
			
	int z=0;
	while(bestpblack[z=rand()%250].x==0)
		;
	
	zl.x=bestpblack[z].x;
	zl.y=bestpblack[z].y;	
	

	return zl;
		
}








