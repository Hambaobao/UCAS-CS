#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2

#define TSIZE 19

#define long_connect 1
#define threeban 2
#define fourban 3
struct point{
	int x;
	int y;
};

struct SingleInfo{		
	int linknum[2];		//连珠数
	int oppnum[2];		//两端对手棋子数量
	int nullnum[2];		//空白位置数量
};

struct SingleScore{
	struct SingleInfo info[8];	//记录8个方向棋子信息
	int score[4];			
};

struct ScoreTableOne{
	int score;			//记录棋盘各点评分
};

struct ScoreTable{
	struct ScoreTableOne Table[4][16][16];
};

struct pointscore{
	int score;
};

struct ScoreTable BlackTable;				//记录棋盘上黑白双方每一点的得分信息
struct ScoreTable WhiteTable;	

struct SingleScore BlackScoreBoard[16][16];		//记录棋盘上黑白双方每一点的棋子信息
struct SingleScore WhiteScoreBoard[16][16];

struct point BlackGradesMax[225];			//记录棋盘上黑白双方棋子得分最高点
struct point WhiteGradesMax[225];

struct point bestpblack[250];				//记录棋盘上黑白双方棋子最优落子位置
struct point bestpwhite[250];

struct pointscore gradestable_black[16][16];
struct pointscore gradestable_white[16][16];

int Three_ban(int s[TSIZE][TSIZE]);
int Four_ban(int s[TSIZE][TSIZE]);
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

int Ban_Hand(int ,int);					//判断禁手
int Long_Connect(int ,int);
int Three_Ban(int ,int);
int Four_Ban(int ,int);


int winner_1(int ,int);					//判断玩家1输赢
int winner_2(int ,int);					//判断玩家2输赢
int IsChessPiece(int i,int j);				//判断当前落子处是否有棋子
int OutChessboard(int i,int j);				//判断当前落子处是否在棋盘内
void recordtoDisplayArray_current(int i,int j);		//用三角形显示当前落子情况
				
void playersvs(void);					//游戏模式
void player_vs_computer_black(void);
void player_vs_computer_white(void);
void test(void);

void Info(int s[SIZE+1][SIZE+1],int m,int n);		//记录棋盘信息
struct ScoreTable arrange_grades_black(void);		//棋盘评分
struct ScoreTable arrange_grades_white(void);

int get_grades_black(int ,int ,int);				//单点评分
int get_grades_white(int ,int ,int);

struct point AI_Black(void);				//电脑（黑棋）
struct point AI_White(void);				//电脑（白棋）
	
//此数组用于记录棋盘格局 
int aRecordBoard[SIZE+1][SIZE+1];		//1_15计数，0行0列置为零，不考虑



//以下变量用于极大极小值算法
int balck_score_board[16][16];			//记录每点四个方向的分数之和
int white_score_board[16][16];

struct pointlist{
	struct point chess[225];
	int len;
};

struct pointlist black_quene;			//储存待选点
struct pointlist white_quene;

struct pointlist black_sure_quene;			//储存最优点
struct pointlist white_sure_quene;

void generate_black_point(void);






