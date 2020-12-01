#include "head.h"
#include  <stdlib.h>

struct point AI_White(void){

	struct point zl;
	int pmax=0;

	int distance=0;
	int dmin=225;


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_white[i][j].score=get_grades_white(i,j);

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax<gradestable_white[i][j].score)
				pmax=gradestable_white[i][j].score;

	for(int i=0;i<250;i++)
		{
			WhiteGradesMax[i].x=0;
			WhiteGradesMax[i].y=0;	
		}

	for(int i=0;i<250;i++)
		{
			bestpwhite[i].x=0;
			bestpwhite[i].y=0;	
		}



	int l=0;									//选出最靠近中心的棋子

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==gradestable_white[i][j].score)
			{
				WhiteGradesMax[l].x=i;
				WhiteGradesMax[l].y=j;
				l++;
			}


	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) < dmin)		//到中心的距离	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) == dmin)
			{
				bestpwhite[l].x=WhiteGradesMax[l].x;
				bestpwhite[l].y=WhiteGradesMax[l].y;
			}
			
	int z=0;
	while(bestpwhite[z=rand()%250].x==0)
		;
	
	zl.x=bestpwhite[z].x;
	zl.y=bestpwhite[z].y;	
	

	return zl;
		
}








