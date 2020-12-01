#include "head.h"

int main()
{
	char model,c;
	//游戏初始化
	system("clear");
	//开始界面显示游戏的基本信息
	printf("-------welcome to silly gobang---------\n");
	printf("-------Writer: Zhang Lei--------\n");
	printf("-------made in China copyrights @ 2017K8009922027\n\n");
	printf("choose model:\n");
	printf("input 1 : player vs player\n");printf("input 2 : player vs computer(black)\n");printf("input 3 : player vs computer(white)\n");printf("input 4 : test model\n");
	scanf("%c",&model);

	//初始化显示出棋盘
	initRecordBorard();
	recordtoDisplayArray();
	displayBoard();		
	printf("-----game start-----\n");
	printf("input q to quilt, and others to continue:");	
	c=getchar();
	if(c=='q'||c=='Q'||c==EOF)
		return 0;
	else
		;
	//选择游戏模式
	switch(model)
	{
		case '1': playersvs();break;				//玩家对战
		case '2': player_vs_computer_black();break;		//人机对战，电脑执黑
		case '3': player_vs_computer_white();break;		//人际对战，电脑执白
		case '4': test();break;					//开发者调试模式
	}
	

	return 0;
}

