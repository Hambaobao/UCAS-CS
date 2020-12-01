#include "head.h"

extern struct SingleScore MyScoreBoard[16][16];

int winner_1(int m,int n){
	for(int i=0;i<4;i++)
	{
		if(MyScoreBoard[m][n].info[i].linknum==4)
			return 1;
	}
	return 0;		
}

int winner_2(int s[SIZE+1][SIZE+1]){
	for(int i=1;i<=SIZE;i++)
	{
		for(int j=1;j<=SIZE+1-5;j++)
		{
			if(s[i][j]==2 && s[i][j+1]==2 && s[i][j+2]==2 && s[i][j+3]==2 && s[i][j+4]==2)
				return 1;
		}		
	}
	for(int j=1;j<=SIZE;j++)
	{
		for(int i=1;i<=SIZE+1-5;i++)
		{
			if(s[i][j]==2 && s[i+1][j]==2 && s[i+2][j]==2 && s[i+3][j]==2 && s[i+4][j]==2)
				return 1;
		}
	}
	for(int i=1;i<=SIZE+1-5;i++)
	{
		for(int j=1;j<=SIZE+1-5;j++)
		{
			if(s[i][j]==2 && s[i+1][j+1]==2 && s[i+2][j+2]==2 && s[i+3][j+3]==2 && s[i+4][j+4]==2)
				return 1;
		}
	}
	for(int i=5;i<=SIZE;i++)
	{
		for(int j=1;j<=SIZE+1-5;j++)
		{
			if(s[i][j]==2 && s[i-1][j+1]==2 && s[i-2][j+2]==2 && s[i-3][j+3]==2 && s[i-4][j+4]==2)
				return 1;
		}
	}
	return 0;	
}

