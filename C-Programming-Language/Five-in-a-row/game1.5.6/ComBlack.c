#include "head.h"

//电脑先手模式
void player_vs_computer_black(void)
{	while(1)
	{
//判断程序继续运行还是退出
	int n,p,a;
	int cnt=0;
	char b,c;
	struct point iposition;
	c=getchar();
	if(c=='q'||c=='Q'||c==EOF)
		break;

//黑子落子

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



	int l=0;

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(pmax==gradestable[i][j].score)
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
			

	
	for(int i=0;i<250;i++)
		if(bestpblack[i].x!=0)
		{
			zl.x=bestpblack[i].x;
			zl.y=bestpblack[i].y;		
		break;
		}

	printf("dmin=%d\n",dmin);
	printf("zl.x=%d zl.y=%d\n",zl.x,zl.y);

				//电脑落子		
			
/*	aRecordBoard[8][8]=1;
	recordtoDisplayArray_current(8,8);
   	displayBoard();
*/	

	aRecordBoard[zl.x][zl.y]=1;
	
	recordtoDisplayArray_current(zl.x,zl.y);
	displayBoard();
	
	
	



	for(int i=1;i<16;i++)
	{
		for(int j=1;j<16;j++)
			printf("%d  ",gradestable[i][j].score);
		printf("\n");
	}
	
		cnt++;

	
	if(winner_1(n,a))		//判断电脑是否赢得比赛
	{
		printf("----Black Win!!!----\n");
		break;
	}






//白子情况
	printf("---player input position:\n");
	scanf("%d",&p);
	scanf("%c",&b);
	for(;IsChessPiece(p,b-'A'+1)||OutChessboard(p,b-'A'+1);)
	{
	if(IsChessPiece(p,b-'A'+1))
	{
		printf("There was a chess piece, please choose other position\n");
	}
	else if(OutChessboard(p,b-'A'+1))
	{
		printf("This position is out of chessboard, please input again\n");
	}
	printf("---player input position:\n");
	scanf("%d",&p);
	scanf("%c",&b);	
	}	
	aRecordBoard[p][b-'A'+1]=2;
	recordtoDisplayArray_current(p,b-'A'+1);
   	displayBoard();

	if(winner_2(p,b-'A'+1))		//判断玩家2是否赢得比赛
	{
		printf("----Player2 Win!!!----\n");
		break;
	}


	recordtoDisplayArray();

	printf("input q to quit, and others to continue");
	c=getchar();

	}
}

