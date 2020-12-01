#include "head.h"

//电脑后手模式
void player_vs_computer_white(void)
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
	
	aRecordBoard[p][b-'A'+1]=1;
	recordtoDisplayArray_current(p,b-'A'+1);
   	displayBoard();


	Info(aRecordBoard,p,b-'A'+1);

	if(winner_1(p,b-'A'+1))								//判断玩家是否赢得比赛
	{
		printf("----Player Win!!!----\n");
		break;
	}

	if(Ban_Hand(p,b-'A'+1)==1)							//判断黑棋禁手
	{
		printf("----Black broke the rule: Long connect !!!----\n");
		printf("input q or Q to cancel, others to continue\n");
		c=getchar();
		if(c=='q'||c=='Q'||c==EOF)
			break;
		
	}			
	else if(Ban_Hand(p,b-'A'+1)==2)
	{
		printf("----Black broke the rule: Three-ban !!!----\n");
		printf("input q or Q to cancel, others to continue\n");
		c=getchar();
		if(c=='q'||c=='Q'||c==EOF)
			break;
	}
	else if(Ban_Hand(p,b-'A'+1)==3)
	{
		printf("----Black broke the rule: Four-ban !!!----\n");
		printf("input q or Q to cancel, others to continue\n");
		c=getchar();
		if(c=='q'||c=='Q'||c==EOF)
			break;
	}




//白子落子
	
	iposition=AI_White();


	for(int i=15;i>0;i--)
	{
		for(int j=1;j<16;j++)
			printf("%-.4d  ",gradestable_white[i][j].score);
		printf("\n");
	}


	printf("iposition.x=%d iposition.y=%d\n",iposition.x,iposition.y);


	aRecordBoard[iposition.x][iposition.y]=2;
	
	recordtoDisplayArray_current(iposition.x,iposition.y);
	displayBoard();

	printf("last:(%d,%c)\n",iposition.x,iposition.y+'A'-1);
	
	Info(aRecordBoard,iposition.x,iposition.y);
	

	
	if(winner_2(iposition.x,iposition.y))						//判断电脑是否赢得比赛
	{
		printf("----Computer Win!!!----\n");
		break;
	}




	recordtoDisplayArray();

	printf("input q to quit, and others to continue");
	c=getchar();

	}
}

