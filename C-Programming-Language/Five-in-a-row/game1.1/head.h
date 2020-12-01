#ifndef _HEAD_
#define _HEAD_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define TSIZE 17
#define CHARSIZE 2

#define long_connect 1
#define threeban 2
#define fourban 3

void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

int winner_1(int ,int );		//判断玩家1输赢
int winner_2(int s[SIZE+1][SIZE+1]);	//判断玩家2输赢
int IsChessPiece(int i,int j);		//判断当前落子处是否有棋子
int OutChessboard(int i,int j);		//判断当前落子处是否在棋盘内
void recordtoDisplayArray_current(int i,int j);		//用三角形显示当前落子情况

void playersvs(void);			//玩家对战

void Info(int s[SIZE+1][SIZE+1]);		//记录棋盘信息


struct SingleInfo{		
	int linknum;		//连珠数
	int oppnum;		//两端对手棋子数量
};

struct SingleScore{
	struct SingleInfo info[4];	//记录4个方向棋子信息
	int score;			//记录该点评分
};

int Ban_Hand(struct [16][16],int m,int n);	//判断禁手

#endif

