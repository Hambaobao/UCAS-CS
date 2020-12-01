#include "head.h"

extern MyScoreBoard;
extern aRecordBoard;

void playersvs(void)
{	while(1)
	{
//判断程序继续运行还是退出
	int n,p;
	char a,b,c;
	c=getchar();
	if(c=='q'||c=='Q'||c==EOF)
		break;
	else
		;

//判断玩家1落子情况
	printf("---player1 input position:\n");
	scanf("%d",&n);
	scanf("%c",&a);
	for(;IsChessPiece(n,a-'A'+1)||OutChessboard(n,a-'A'+1);)
	{
	if(IsChessPiece(n,a-'A'+1))
		{
		printf("There was a chess piece, please choose other position\n");
		}
	else if(OutChessboard(n,a-'A'+1))
		{
		printf("This position is out of chessboard, please input again\n");
		}
	printf("---player1 input position:\n");
	scanf("%d",&n);
	scanf("%c",&a);	
	}

	Info(aRecordBoard);				//获取棋盘信息
	if(Ban_Hand(MyScoreBoard,n,a-'A'+1)==1)				//判断禁手
	{
	printf("----break the rule: Long connect !!!----\n");
	break;
	}			
	else if(Ban_Hand(MyScoreBoard,n,a-'A'+1)==2)
	{
		printf("----break the rule: Three-ban !!!----\n");
		break;
	}
	else if(Ban_Hand(MyScoreBoard,n,a-'A'+1)==3)
	{
		printf("----break the rule: Four-ban !!!----\n");
		break;
	}

	aRecordBoard[n][a-'A'+1]=1;
	
	recordtoDisplayArray_current(n,a-'A'+1);
   	displayBoard();
	
	if(winner_1(n,a-'A'+1))		//判断玩家1是否赢得比赛
	{
		printf("----Player1 Win!!!----\n");
		break;
	}

//判断玩家2落子情况
	printf("---player2 input position:\n");
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
	printf("---player2 input position:\n");
	scanf("%d",&p);
	scanf("%c",&b);	
	}	
	aRecordBoard[p][b-'A'+1]=2;
	recordtoDisplayArray_current(p,b-'A'+1);
   	displayBoard();

	if(winner_2(aRecordBoard))		//判断玩家2是否赢得比赛
	{
		printf("----Player2 Win!!!----\n");
		break;
	}

	recordtoDisplayArray();

	printf("input q to quit, and others to continue");
	c=getchar();
	}
}

int IsChessPiece(int i,int j)
{
	if(aRecordBoard[i][j]==1||aRecordBoard[i][j]==2)
		return 1;
	else
		return 0;
}

int OutChessboard(int i,int j)
{
	if((1<=i&&i<=15)&&(1<=j&&j<=15))
		return 0;
	else
		return 1;
}



