#include "head.h"

//电脑先手模式
void player_vs_computer_black(void)
{	while(1)
	{
//判断程序继续运行还是退出
	int n,p,a;

	char b,c;
	struct point iposition;
	c=getchar();
	if(c=='q'||c=='Q'||c==EOF)
		break;

//黑子落子
	
	iposition=AI_Black();

	printf("iposition.x=%d iposition.y=%d\n",iposition.x,iposition.y);


	aRecordBoard[iposition.x][iposition.y]=1;
	
	recordtoDisplayArray_current(iposition.x,iposition.y);
	displayBoard();
	
	printf("last:(%d,%c)\n",iposition.x,iposition.y+'A'-1);

	Info(aRecordBoard,iposition.x,iposition.y);
	
/*
	for(int i=15;i>0;i--)
	{
		for(int j=1;j<16;j++)
			printf("%-.4d  ",gradestable_black[i][j].score);
		printf("\n");
	}
	
*/
	
	if(winner_1(iposition.x,iposition.y))		//判断电脑是否赢得比赛
	{
		printf("----Computer Win!!!----\n");
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

	Info(aRecordBoard,p,b-'A'+1);

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

