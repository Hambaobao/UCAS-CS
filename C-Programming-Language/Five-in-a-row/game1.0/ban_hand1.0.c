#include <stdio.h>
#define SIZE 15
#define TSIZE 17

#define long_connect 1
#define threeban 2
#define fourban 3

struct SingleInfo{		
	int linknum;		//连珠数
	int oppnum;		//两端对手棋子数量
};

struct SingleScore{
	struct SingleInfo info[4];	//记录4个方向棋子信息
	int score;			//记录该点评分
};

struct SingleScore MyScoreBoard[15][15];		//记录棋盘上每一点的得分
struct SingleScore OppScoreBoard[15][15];

int Three_ban(int s[TSIZE][TSIZE]);
int Four_ban(int s[TSIZE][TSIZE]);


int Ban_Hand(int s[SIZE+1][SIZE+1],int m,int n)
{
	s[m][n] = 1;

	int temp[TSIZE][TSIZE];			//声明一个大棋盘，使其完全容纳住小棋盘
	for(int i=0;i<TSIZE;i++)
		for(int j=0;j<TSIZE;j++)
			temp[i][j]=2;		//初始化大棋盘边界，对黑子来说，2是白子也是棋盘外部，均不可落子

	for(int i=1;i<=SIZE;i++)
		for(int j=1;j<=SIZE;j++)
			temp[i][j]=s[i][j];	//将小棋盘的值复制到大棋盘正中央，使其周围被厚厚的边界包裹

	for(int i=0;temp[m+i][n]==1;i++) MyScoreBoard[m][n].info[0].linknum++;		//记录——向连珠数
	for(int i=1;temp[m-i][n]==1;i++) MyScoreBoard[m][n].info[0].linknum++;
										
	for(int i=0;temp[m+i][n+i]==1;i++) MyScoreBoard[m][n].info[1].linknum++;		//记录 / 向连珠数
	for(int i=1;temp[m-i][n-i]==1;i++) MyScoreBoard[m][n].info[1].linknum++;

	for(int i=0;temp[m][n+i]==1;i++) MyScoreBoard[m][n].info[2].linknum++;		//记录 | 向连珠数
	for(int i=1;temp[m+i][n-i]==1;i++) MyScoreBoard[m][n].info[2].linknum++;

	for(int i=0;temp[m-i][n+i]==1;i++) MyScoreBoard[m][n].info[3].linknum++;		//记录 \ 向连珠数
	for(int i=1;temp[m+i][n-i]==1;i++) MyScoreBoard[m][n].info[3].linknum++;

	int i;
	for(i=1;temp[m+i][n]==1;i++)	;	if(temp[m+i][n]==2)	MyScoreBoard[m][n].info[0].oppnum++;		//记录——向对手棋子数
	for(i=1;temp[m-i][n]==1;i++)	;	if(temp[m-i][n]==2)	MyScoreBoard[m][n].info[0].oppnum++;

	for(i=1;temp[m+i][n+i]==1;i++)	;	if(temp[m+i][n+i]==2)	MyScoreBoard[m][n].info[1].oppnum++;		//记录/向对手棋子数
	for(i=1;temp[m-i][n-i]==1;i++)	;	if(temp[m-i][n-i]==2)	MyScoreBoard[m][n].info[1].oppnum++;

	for(i=1;temp[m][n+i]==1;i++)	;	if(temp[m][n+i]==2)	MyScoreBoard[m][n].info[2].oppnum++;		//记录 | 向对手棋子数
	for(i=1;temp[m][n-i]==1;i++)	;	if(temp[m][n-i]==2)	MyScoreBoard[m][n].info[2].oppnum++;

	for(i=1;temp[m-i][n+i]==1;i++)	;	if(temp[m-i][n+i]==2)	MyScoreBoard[m][n].info[3].oppnum++;		//记录\向对手棋子数
	for(i=1;temp[m+i][n-i]==1;i++)	;	if(temp[m+i][n-i]==2)	MyScoreBoard[m][n].info[3].oppnum++;
	
		
	int a;
	for(a=0;MyScoreBoard[m][n].info[a].linknum>5;a++)	;			//判断长连禁手	
		return (a>0)?long_connect:0;

//	if(Three_ban(temp))
//		return threeban;
	
//	if(Four_ban(temp))
//		return fourban;

	return 0;
}


