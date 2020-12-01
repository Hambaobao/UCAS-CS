#include "head.h"
#include  <stdlib.h>
																	//电脑黑棋
struct point AI_Black_0(int n){
	int candidates_black(long long int []);//用于选出待选点，存到MAX数组
	struct point zl;
	long long int pmax[8];//用于记录最高的几个分数
																	//找到的分最高的位置
	int l;	
	int s=0,w;

	l=candidates_black(pmax);	

	struct point val;//用来记录每一次落子后，对手返回的棋子的位置和最高的分
	int x,y;//用来暂时记录虚拟下如的棋子的位置
	long long int tempmax=-34867844010*4;//用来记录试探一步后的最高的分，开始赋一个最小值
	if(n>0)
	{
		for(int i=0;i<l;i++)													//把每一个得分较高的子都下一遍
		{
			x=BlackGradesMax[i].x;
			y=BlackGradesMax[i].y;
			aRecordBoard[x][y]=1;//落下一颗棋子
	
			Info(aRecordBoard,x,y);

			if(winner_1(x,y))
			{
				aRecordBoard[x][y]=0;//还原棋盘
				zl.x=x;
				zl.y=y;
				zl.score=34867844010;
				return zl;
			}

			val=AI_White_0(n-1);
			
			if(val.score==tempmax)//如果找到和当前最高分数一样的点就保存下来
			{
				bestpblack[s].x=x;
				bestpblack[s].y=y;
				s++;	
			}

			if(val.score>tempmax)//如果找到比当前最高的分高的分数，就把之前点清空;
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
	zl.score=pmax[0];	
	
	return zl;
		
}

																	//电脑白棋
struct point AI_Black_1(int n){
	int candidates_black(long long int []);//用于选出待选点，存到MAX数组
	struct point zl;
	long long int pmax[8];//用于记录最高的几个分数
																	//找到的分最高的位置
	int l;	
	int s=0,w;

	l=candidates_black(pmax);	

	struct point val;//用来记录每一次落子后，对手返回的棋子的位置和最高的分
	int x,y;//用来暂时记录虚拟下如的棋子的位置
	long long int tempmax=-34867844010*4;//用来记录试探一步后的最高的分，开始赋一个最小值
	if(n>0)
	{
		for(int i=0;i<l;i++)													//把每一个得分较高的子都下一遍
		{
			x=BlackGradesMax[i].x;
			y=BlackGradesMax[i].y;
			aRecordBoard[x][y]=1;//落下一颗棋子

			Info(aRecordBoard,x,y);

			if(winner_1(x,y))
			{
				aRecordBoard[x][y]=0;//还原棋盘
				zl.x=x;
				zl.y=y;
				zl.score=200000000;
				return zl;
			}

			val=AI_White_1(n-1);
			
			if(val.score==tempmax)//如果找到和当前最高分数一样的点就保存下来
			{
				bestpblack[s].x=x;
				bestpblack[s].y=y;
				s++;	
			}

			if(val.score>tempmax)//如果找到比当前最高的分高的分数，就把之前点清空;
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
	zl.score=pmax[0];	
	
	return zl;
		
}

int candidates_black(long long int pmax[8]){

	int count=0;//选出8个待选点，初始为0

	for(int i=0;i<8;i++)
		pmax[i]=0;

	for(int i=0;i<250;i++)
		{
			BlackGradesMax[i].x=0;
			BlackGradesMax[i].y=0;	
		}

	for(int i=0;i<250;i++)
		{
			bestpblack[i].x=-1;
			bestpblack[i].y=-1;	
		}


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_black[i][j].score=get_grades_black(i,j);
																//找出最高的分
	int g;
	while(count<8){
		for(int i=1;i<16;i++)
			for(int j=1;j<16;j++)
				if(pmax[0]<gradestable_black[i][j].score)
					pmax[0]=gradestable_black[i][j].score;

		for(int i=1;i<16;i++)
			for(int j=1;j<16;j++)
				if(pmax[0]==gradestable_black[i][j].score)
				{
					BlackGradesMax[count].x=i;
					BlackGradesMax[count].y=j;
					count++;
				}
		if(count>7)
			break;
	
		for(int k=1;count<8;k++)
		{
			for(int i=1;i<16;i++)
				for(int j=1;j<16;j++)
					if(pmax[k]<(g=gradestable_black[i][j].score) && g<pmax[k-1])
						pmax[k]=gradestable_black[i][j].score;
	
			for(int i=1;i<16;i++)
				for(int j=1;j<16;j++)
					if(pmax[k]==gradestable_black[i][j].score)
					{
						BlackGradesMax[count].x=i;
						BlackGradesMax[count].y=j;
						count++;
					}		
		}
	}

	return count;		
}







