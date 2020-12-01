#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2

#define TSIZE 19

#define long_connect 1
#define threeban 2
#define fourban 3

struct SingleInfo{		
	int linknum[2];		//连珠数
	int oppnum[2];		//两端对手棋子数量
	int nullnum[2];		//空白位置数量
};

struct SingleScore{
	struct SingleInfo info[8];	//记录8个方向棋子信息
	int score;			//记录该点评分
};

struct SingleScore BlackScoreBoard[16][16];		//记录棋盘上黑白双方每一点的得分和信息
struct SingleScore WhiteScoreBoard[16][16];

int Three_ban(int s[TSIZE][TSIZE]);
int Four_ban(int s[TSIZE][TSIZE]);
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

int Ban_Hand(int x,int y);//判断禁手

int winner_1(int ,int);	//判断玩家1输赢
int winner_2(int ,int);	//判断玩家2输赢
int IsChessPiece(int i,int j);		//判断当前落子处是否有棋子
int OutChessboard(int i,int j);		//判断当前落子处是否在棋盘内
void recordtoDisplayArray_current(int i,int j);		//用三角形显示当前落子情况
				
void playersvs(void);			//游戏模式
void test(void);

void Info(int s[SIZE+1][SIZE+1],int m,int n);	//记录棋盘信息

//此数组用于记录棋盘格局 
int aRecordBoard[SIZE+1][SIZE+1];		//1_15计数，0行0列置为零，不考虑







