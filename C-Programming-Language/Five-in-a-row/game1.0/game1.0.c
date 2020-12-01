#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

int winner_1(int s[SIZE+1][SIZE+1]);	//判断玩家1输赢
int winner_2(int s[SIZE+1][SIZE+1]);	//判断玩家2输赢
int IsChessPiece(int i,int j);		//判断当前落子处是否有棋子
int OutChessboard(int i,int j);		//判断当前落子处是否在棋盘内
void recordtoDisplayArray_current(int i,int j);		//用三角形显示当前落子情况
void playersvs(void);

int Ban_Hand(int s[SIZE+1][SIZE+1],int m,int n);	//判断禁手

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


int main()
{
	char model,c;
	//游戏初始化
	system("clear");
	printf("choose model:\n");
	printf("input 1 : player vs player\n");printf("input 2 : player vs computer(black)\n");printf("input 3 : player vs computer(white)\n");
	scanf("%c",&model);

	initRecordBorard();
	recordtoDisplayArray();
	displayBoard();		
	printf("-----game start-----\n");
	printf("input q to quilt, and others to continue\n");	
	c=getchar();
	if(c=='q'||c=='Q'||c==EOF)
		return 0;
	else
		;
	switch(model)
	{
		case '1': playersvs();break;
//		case '2': player_vs_computer_black();break;
//		case '3': player_vs_computer_white();break; 
	}
	

	return 0;
}

//初始化棋盘格局 
void initRecordBorard(void){
//通过双重循环，将aRecordBoard清0
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE;j++)
			aRecordBoard[i][j]=0;
}

//将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
void recordtoDisplayArray(void){
//第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE*CHARSIZE+1+2;j++)
			aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
//第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
	for(int i=1;i<SIZE+1;i++)
		for(int j=1;j<SIZE+1;j++)
			if(aRecordBoard[i][j]==1)
				{
				for(int k=0;k<2;k++)
						{						
					aDisplayBoardArray[15-i][j*2+k]=play1Pic[k];
							}
				}
			else if(aRecordBoard[i][j]==2)
				{
				for(int k=0;k<2;k++)
						{						
					aDisplayBoardArray[15-i][j*2+k]=play2Pic[k];
							}
				}
//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
}


void recordtoDisplayArray_current(int i,int j){
//第一步：扫描新输入到aRecordBoard的元素，当遇到非0的元素，将▲或者△复制到aDisplayBoardArray的相应位置上
			if(aRecordBoard[i][j]==1)
				{
				for(int k=0;k<2;k++)
						{						
					aDisplayBoardArray[15-i][j*2+k]=play1CurrentPic[k];
							}
				}
			else if(aRecordBoard[i][j]==2)
				{
				for(int k=0;k<2;k++)
						{						
					aDisplayBoardArray[15-i][j*2+k]=play2CurrentPic[k];
							}
				}
//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。▲和△也是中文字符，每个也占2个字节。
}

//显示棋盘格局 
void displayBoard(void){
	int i;
	//第一步：清屏
	system("clear");   //清屏  
	//第二步：将aDisplayBoardArray输出到屏幕上
	for(int i=0;i<SIZE;i++)
		{
		for(int j=0;j<SIZE*CHARSIZE+1+2;j++)
			printf("%c",aDisplayBoardArray[i][j]);
		printf("\n");		
		}
	
	//第三步：输出最下面的一行字母A B .... 
	printf("  %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n",'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O');
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

int winner_1(int s[SIZE+1][SIZE+1]){
	for(int i=1;i<=SIZE;i++)
	{
		for(int j=1;j<=SIZE+1-5;j++)
		{
			if(s[i][j]==1 && s[i][j+1]==1 && s[i][j+2]==1 && s[i][j+3]==1 && s[i][j+4]==1)
				return 1;
		}		
	}
	for(int j=1;j<=SIZE;j++)
	{
		for(int i=1;i<=SIZE+1-5;i++)
		{
			if(s[i][j]==1 && s[i+1][j]==1 && s[i+2][j]==1 && s[i+3][j]==1 && s[i+4][j]==1)
				return 1;
		}
	}
	for(int i=1;i<=SIZE+1-5;i++)
	{
		for(int j=1;j<=SIZE+1-5;j++)
		{
			if(s[i][j]==1 && s[i+1][j+1]==1 && s[i+2][j+2]==1 && s[i+3][j+3]==1 && s[i+4][j+4]==1)
				return 1;
		}
	}
	for(int i=5;i<=SIZE;i++)
	{
		for(int j=1;j<=SIZE+1-5;j++)
		{
			if(s[i][j]==1 && s[i-1][j+1]==1 && s[i-2][j+2]==1 && s[i-3][j+3]==1 && s[i-4][j+4]==1)
				return 1;
		}
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
	if(Ban_Hand(aRecordBoard,n,a-'A'+1)==1)				//判断禁手
	{
	printf("----break the rule: Long connect !!!----\n");
	break;
	}			
	else if(Ban_Hand(aRecordBoard,n,a-'A'+1)==2)
	{
		printf("----break the rule: Three-ban !!!----\n");
		break;
	}
	else if(Ban_Hand(aRecordBoard,n,a-'A'+1)==3)
	{
		printf("----break the rule: Four-ban !!!----\n");
		break;
	}

	aRecordBoard[n][a-'A'+1]=1;
	
	recordtoDisplayArray_current(n,a-'A'+1);
   	displayBoard();

	if(winner_1(aRecordBoard))		//判断玩家1是否赢得比赛
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





