#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);


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
int aRecordBoard[SIZE+1][SIZE+1];


int main()

{
	initRecordBorard();
	recordtoDisplayArray();
	displayBoard();
	while(1)
		{
//判断程序继续运行还是退出
		int n,p;
		char a,b,c;
		c=getchar();
		if(c=='q'||c==EOF)
			break;
		else
			;

//判断玩家1落子情况
		printf("player1 input position:\n");
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
			printf("player1 input position:\n");
			scanf("%d",&n);
			scanf("%c",&a);	
			}
		aRecordBoard[n][a-'A'+1]=1;
		recordtoDisplayArray_current(n,a-'A'+1);
    		displayBoard();

//判断玩家2落子情况
		printf("player2 input position:\n");
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
			printf("player2 input position:\n");
			scanf("%d",&p);
			scanf("%c",&b);	
			}
	
//成功落子后更新棋盘
		aRecordBoard[p][b-'A'+1]=2;
		recordtoDisplayArray_current(p,b-'A'+1);
    		displayBoard();

		recordtoDisplayArray();

		printf("input q to quit, and others to continue\n");
		c=getchar();
		}





  /*initRecordBorard();
    recordtoDisplayArray();
    displayBoard();
    getchar();

    aRecordBoard[5][8]=1;
    aRecordBoard[5][9]=2;
    recordtoDisplayArray();
    displayBoard();
    getchar();

    aRecordBoard[3][4]=2;
    recordtoDisplayArray();
    displayBoard();
    getchar();

    aRecordBoard[6][1]=1;
    recordtoDisplayArray();
    displayBoard();
    getchar();

    aRecordBoard[9][4]=2;
    recordtoDisplayArray();
    displayBoard();
    getchar();*/
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
















