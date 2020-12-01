#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2

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

struct SingleScore MyScoreBoard[16][16];		//记录棋盘上每一点的得分
struct SingleScore OppScoreBoard[16][16];

int Three_ban(int s[TSIZE][TSIZE]);
int Four_ban(int s[TSIZE][TSIZE]);
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

int winner_1(int ,int);	//判断玩家1输赢
int winner_2(int ,int);	//判断玩家2输赢
int IsChessPiece(int i,int j);		//判断当前落子处是否有棋子
int OutChessboard(int i,int j);		//判断当前落子处是否在棋盘内
void recordtoDisplayArray_current(int i,int j);		//用三角形显示当前落子情况
void playersvs(void);

void Info(int s[SIZE+1][SIZE+1]);	//记录棋盘信息

//棋盘使用的是GBK编码，每一个中文字符占用2个字节。

//棋盘基本模板 
char aInitDisplayBoardArray[SIZE][SIZE*CHARSIZE+1+2] = 
{
		"15┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"14┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"13┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"12┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"11┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"10┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 9┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 8┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 7┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 6┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 5┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 4┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 3┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 2┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		" 1┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组用于显示棋盘 
char aDisplayBoardArray[SIZE][SIZE*CHARSIZE+1+2];
 
char play1Pic[]="●";//黑棋子;
char play1CurrentPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurrentPic[]="△";

//此数组用于记录棋盘格局 
int aRecordBoard[SIZE+1][SIZE+1];		//1_15计数，0行0列置为零，不考虑

