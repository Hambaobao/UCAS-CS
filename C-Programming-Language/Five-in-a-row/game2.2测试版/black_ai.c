#include "head.h"

//选出得分最高 && 能有效切断对方棋子 && 最靠近中心的棋子

struct scorelist AI(void){

	int get_opp_grades(int ,int);
	struct scorelist zl;
	struct scorelist mylist[250];
	int pmax=0;

	int distance=0;
	int dmin=225;

																		//获取棋盘各点的评分
	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable[i][j].score=get_grades_black(i,j);

	for(int i=1;i<16;i++)															//找出最高的评分
		for(int j=1;j<16;j++)
			if(pmax<gradestable[i][j].score)
				pmax=gradestable[i][j].score;

	for(int i=0;i<250;i++)															//清零
		{
			BlackGradesMax[i].x=0;
			BlackGradesMax[i].y=0;	
		}

	for(int i=0;i<250;i++)															
		{	
			bestpblack[i].x=0;
			bestpblack[i].y=0;	
		}



	int l=0;																

	for(int i=1;i<16;i++)															//将得分最高的点放入备选数组
		for(int j=1;j<16;j++)
			if(pmax==gradestable[i][j].score)
			{
				BlackGradesMax[l].x=i;
				BlackGradesMax[l].y=j;
				l++;
			}

	zl.score=pmax;

	for(int l=0;l<250;l++)															//获取备选点落对方棋子时对方的得分
		mylist[l].score=get_opp_grades(BlackGradesMax[l].x,BlackGradesMax[l].y);

	pmax=0;
	for(int l=0;l<250;l++)															//找出对方的最高得分
			if(pmax<mylist[l].score)
				pmax=mylist[l].score;

	for(int i=0;i<250;i++)															//清零
		{
			WhiteGradesMax[i].x=0;
			WhiteGradesMax[i].y=0;	
		}

	for(int i=0;i<250;i++)															
		{	
			bestpwhite[i].x=0;
			bestpwhite[i].y=0;	
		}

	l=0;
	for(int i=1;i<16;i++)															//将得分最高的点放入备选数组
		for(int j=1;j<16;j++)
			if(pmax==mylist[l].score)
			{
				WhiteGradesMax[l].x=i;
				WhiteGradesMax[l].y=j;
				l++;
			}			


	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) < dmin)		//备选点到中心的距离	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) == dmin)
			{
				bestpblack[l].x=WhiteGradesMax[l].x;
				bestpblack[l].y=WhiteGradesMax[l].y;
			}
			

	for(int i=0;i<250;i++)
		if(bestpblack[i].x!=0)
		{
			zl.x=bestpblack[i].x;
			zl.y=bestpblack[i].y;		
		break;
		}

	return zl;
		
}








