#include "head.h"
#include  <stdlib.h>

struct point AI_minmax_Black(int n){
	struct point zl;
	long long int pmax=0;

	int distance=0;
	int dmin=225;


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_black[i][j].score=get_grades_black(i,j);
																	//找出最高的分
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


	int l=0;																//找到的分最高的位置

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==gradestable_black[i][j].score)
			{
				BlackGradesMax[l].x=i;
				BlackGradesMax[l].y=j;
				l++;
			}


	int s=0,w;
	struct point val;//用来记录每一次落子后，对手返回的棋子的位置和最高的分
	int x,y;//用来暂时记录虚拟下如的棋子的位置
	long long int tempmax=34867844010*4;//用来记录试探一步后的最高的分，开始赋一个最大值
	if(n>0)
	{
		for(int i=0;i<l;i++)													//把每一个得分最高的子都下一遍
		{
			x=BlackGradesMax[i].x;
			y=BlackGradesMax[i].y;
			aRecordBoard[x][y]=1;//落下一颗棋子

			if(winner_1(x,y))
			{
				aRecordBoard[x][y]=0;//还原棋盘
				zl.x=x;
				zl.y=y;
				zl.score=34867844010;
				return zl;
			}

			val=AI_White(n-1);
			
			if(val.score==tempmax)//如果找到和当前最高分数一样的点就保存下来
			{
				bestpblack[s].x=x;
				bestpblack[s].y=y;
				s++;	
			}

			if(val.score<tempmax)//如果找到比当前最高的分低的分数，就把之前点清空;
			{
				s=0;
				tempmax=val.score;
				bestpblack[s].x=x;
				bestpblack[s].y=y;
				s++;	
			}
			aRecordBoard[x][y]=0;//还原棋盘
			
		}
		w=(rand()%s);				

		zl.x=bestpblack[w].x;
		zl.y=bestpblack[w].y;
		zl.score=tempmax;	
	
		return zl;
		
	}	



/*
	for(int l=0;l<250;l++)
		if((distance = (BlackGradesMax[l].x-8)*(BlackGradesMax[l].x-8)+(BlackGradesMax[l].y-8)*(BlackGradesMax[l].y-8)) < dmin)		//到中心的距离	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (BlackGradesMax[l].x-8)*(BlackGradesMax[l].x-8)+(BlackGradesMax[l].y-8)*(BlackGradesMax[l].y-8)) == dmin)
			{
				bestpblack[l].x=BlackGradesMax[l].x;
				bestpblack[l].y=BlackGradesMax[l].y;
			}
*/
			
	int z;
	z=(rand()%l);				//如果有多个最优的点就随机下一个

	zl.x=BlackGradesMax[z].x;
	zl.y=BlackGradesMax[z].y;
	zl.score=pmax;	
	
	return zl;
		
}


struct point AI_White(int n){

	struct point zl;
	long long int pmax=0;

	int distance=0;
	int dmin=225;


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_white[i][j].score=get_grades_white(i,j);

																		//找出最高的分
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



	int l=0;																//选出得分最高的位置

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==gradestable_white[i][j].score)
			{
				WhiteGradesMax[l].x=i;
				WhiteGradesMax[l].y=j;
				l++;
			}

	int s=0,w;
	struct point val;//用来记录每一次落子后，对手返回的棋子的位置和最高的分
	int x,y;//用来暂时记录虚拟下如的棋子的位置
	long long int tempmax=-34867844010*4;//用来记录试探一步后的最高的分，开始赋一个最小值
	if(n>0)
	{
		for(int i=0;i<l;i++)													//把每一个得分最高的子都下一遍
		{
			x=WhiteGradesMax[i].x;
			y=WhiteGradesMax[i].y;
			aRecordBoard[x][y]=2;//落下一颗棋子
/*
			if(winner_2(x,y))
			{
				aRecordBoard[x][y]=0;//还原棋盘
				zl.x=x;
				zl.y=y;
				zl.score=34867844010;
				return zl;
			}
*/
			val=AI_Black(n-1);
			
			if(val.score==tempmax)//如果找到和当前最高分数一样的点就保存下来
			{
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}

			if(val.score>tempmax)//如果找到比当前最高的分低的分数，就把之前点清空;
			{
				s=0;
				tempmax=val.score;
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}
			aRecordBoard[x][y]=0;//还原棋盘
			
		}
		w=(rand()%s);					

		zl.x=bestpwhite[w].x;
		zl.y=bestpwhite[w].y;
		zl.score=tempmax;	

		return zl;
	
	}	


/*
	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) < dmin)		//到中心的距离	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) == dmin)
			{
				bestpwhite[l].x=WhiteGradesMax[l].x;
				bestpwhite[l].y=WhiteGradesMax[l].y;
			}
*/
			
	int z;
	z=(rand()%l);					//如果有多个最优的点就随机下一个

	zl.x=WhiteGradesMax[z].x;
	zl.y=WhiteGradesMax[z].y;
	zl.score=pmax;	

	return zl;
		
}







