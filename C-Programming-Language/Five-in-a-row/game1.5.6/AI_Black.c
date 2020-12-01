#include "head.h"

struct point AI_Black(void){
	void choosepoints(void);
	struct point p;
	choosepoints();
	for(int i=0;i<10;i++)
		if(BlackGradesMax[i].x!=0)
		{
			p.x=BlackGradesMax[i].x;
			p.y=BlackGradesMax[i].y;		
		break;
		}
		
	return p;
	
}

void choosepoints(void){
	int pmax=0;	
	for(int i=0;i<250;i++)
		BlackGradesMax[i].x=0,BlackGradesMax[i].y=0;

	BlackTable=arrange_grades();

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax<BlackTable.Table[i][j].score)
				pmax=BlackTable.Table[i][j].score;

	int l=0;

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==BlackTable.Table[i][j].score)
			{
				BlackGradesMax[l].x=i;
				BlackGradesMax[l].y=j;
				l++;
			}
	
}
