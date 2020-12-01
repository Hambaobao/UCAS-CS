#include "head.h"


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

//ÅÐ¶ÏÊäÓ®
int winner_1(int m,int n){
	for(int i=0;i<4;i++)
	if(MyScoreBoard[m][n].info[i].linknum==4)
		return 1;
	return 0;
}

int winner_2(int m,int n){
	for(int i=0;i<4;i++)
	if(OppScoreBoard[m][n].info[i].linknum==4)
		return 1;
	return 0;
}

