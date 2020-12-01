#include "head.h"
#include  <stdlib.h>
																	//电脑黑棋
struct point AI_White_0(int n){
	int candidates_white(long long int [8],struct point [250]);//用于选出待选点，存到MAX数组
	struct point zl;
	long long int pmax[8];//用于记录最高的几个分数
	struct point bestpwhite[250];		//记录棋盘上黑白双方棋子最优落子位置
	struct point WhiteGradesMax[250];	//记录棋盘上黑白双方棋子得分最高点

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
																	//找到的分最高的位置
	int l;	
	int s=0,w;

	l=candidates_white(pmax,WhiteGradesMax);	

	struct point val;//用来记录每一次落子后，对手返回的棋子的位置和最高的分
	int x,y;//用来暂时记录虚拟下如的棋子的位置
	long long int tempmax=-549755813888*4;//用来记录试探一步后的最高的分，开始赋一个最小值
	if(n>0)
	{
		for(int i=0;i<l;i++)													//把每一个得分较高的子都下一遍
		{
			x=WhiteGradesMax[i].x;
			y=WhiteGradesMax[i].y;
			aRecordBoard[x][y]=2;//落下一颗棋子
		
			Info(aRecordBoard,x,y);

			if(winner_2(x,y))
			{
				aRecordBoard[x][y]=0;//还原棋盘
				zl.x=x;
				zl.y=y;
				zl.score=549755813888;
				return zl;
			}
	
			val=AI_Black_0(n-1);//递归调用AI
		
			if(val.score==tempmax)//如果找到和当前最高分数一样的点就保存下来
			{
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}

			if(val.score>tempmax)//如果找到比当前最高的分高的分数，就把之前点清空;
			{
				s=0;
				tempmax=val.score;
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}
			aRecordBoard[x][y]=0;//还原棋盘
			
		}
		w=(rand()%s);			//在最后确定的几个点里随机选一个下			

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
	z=(rand()%4);					//如果有多个最优的点就随机下一个

	zl.x=WhiteGradesMax[z].x;
	zl.y=WhiteGradesMax[z].y;
	zl.score=pmax[0];	

	return zl;
		
}


																		//电脑白棋
struct point AI_White_1(int n){
	int candidates_white(long long int [8],struct point [250]);//用于选出待选点，存到MAX数组
	struct point zl;
	long long int pmax[8];//用于记录最高的几个分数
	struct point bestpwhite[250];			//记录棋盘上黑白双方棋子最优落子位置
	struct point WhiteGradesMax[250];		//记录棋盘上黑白双方棋子得分最高点

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
																	//找到的分最高的位置
	int l;	
	int s=0,w;

	l=candidates_white(pmax,WhiteGradesMax);	

	struct point val;//用来记录每一次落子后，对手返回的棋子的位置和最高的分
	int x,y;//用来暂时记录虚拟下如的棋子的位置
	long long int tempmax=549755813888*4;//用来记录试探一步后的最高的分，开始赋一个最大值
	if(n>0)
	{
		for(int i=0;i<l;i++)													//把每一个得分较高的子都下一遍
		{
			x=WhiteGradesMax[i].x;
			y=WhiteGradesMax[i].y;
			aRecordBoard[x][y]=2;//落下一颗棋子
/*
			Info(aRecordBoard,x,y);

			if(winner_2(x,y))
			{
				aRecordBoard[x][y]=0;//还原棋盘
				zl.x=x;
				zl.y=y;
				zl.score=34867844010;
				return zl;
			}
*/
			val=AI_Black_1(n-1);//递归调用AI
			
			if(val.score==tempmax)//如果找到和当前最高分数一样的点就保存下来
			{
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}

			if(val.score<tempmax)//如果找到比当前最高的分低的分数，就把之前点清空;
			{
				s=0;
				tempmax=val.score;
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}
			aRecordBoard[x][y]=0;//还原棋盘
			
		}
		w=(rand()%s);				//在最后确定的几个点里随机选一个下				

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
	z=(rand()%4);					//如果有多个最优的点就随机下一个

	zl.x=WhiteGradesMax[z].x;
	zl.y=WhiteGradesMax[z].y;
	zl.score=pmax[0];	

	return zl;
		
}

//产生白棋的待选落子位置
int candidates_white(long long int pmax[8],struct point WhiteGradesMax[250]){

	int count=0;//选出6个待选点，初始为0

	for(int i=0;i<6;i++)
		pmax[i]=0;

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_white[i][j].score=get_grades_white(i,j);
																//找出最高的分
	int g;
	while(count<6){
		for(int i=1;i<16;i++)
			for(int j=1;j<16;j++)
				if(pmax[0]<gradestable_white[i][j].score)
					pmax[0]=gradestable_white[i][j].score;

		for(int i=1;i<16;i++)
			for(int j=1;j<16;j++)
				if(pmax[0]==gradestable_white[i][j].score)
				{
					WhiteGradesMax[count].x=i;
					WhiteGradesMax[count].y=j;
					count++;
				}
		if(count>5)											//如果不够6个，就接着找
			break;
	
		for(int k=1;count<6;k++)
		{
			for(int i=1;i<16;i++)
				for(int j=1;j<16;j++)
					if(pmax[k]<(g=gradestable_white[i][j].score) && g<pmax[k-1])
						pmax[k]=gradestable_white[i][j].score;
	
			for(int i=1;i<16;i++)
				for(int j=1;j<16;j++)
					if(pmax[k]==gradestable_white[i][j].score)
					{
						WhiteGradesMax[count].x=i;
						WhiteGradesMax[count].y=j;
						count++;
					}		
		}
	}

	return count;		
}











