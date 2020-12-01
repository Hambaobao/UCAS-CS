#include "head.h"

// long_connect 1
// threeban 2
// fourban 3

int Ban_Hand(int x,int y){
	int Long_Connect(int ,int);
	int Three_Ban(int ,int);
	int Four_Ban(int ,int);

	int s[16][16];
	for(int i=0;i<16;i++)
		for(int j=0;j<16;j++)
			s[i][j]=aRecordBoard[i][j];		//复制当前棋局
	
	s[x][y]=1;			//填入试探棋子

	Info(s,x,y);			//获取落子处棋盘信息

	if(Long_Connect(x,y))
		return long_connect;

	if(Three_Ban(x,y))
		return threeban;

	if(Four_Ban(x,y))
		return fourban;

	return 0;
}

//长连禁手判断
int Long_Connect(int m,int n){
	int j=0;
	for(int i=0;i<4;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[i+4].linknum[0]>5)
			j++;
	return (j>0)?1:0;	
}

//33禁手判断
int Three_Ban(int m,int n){
	int three=0;
	for(int i=0;i<8;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]+BlackScoreBoard[m][n].info[i].linknum[1]==3
			&& BlackScoreBoard[m][n].info[i].nullnum[0]==1 && BlackScoreBoard[m][n].info[i].nullnum[1]>0
			&& BlackScoreBoard[m][n].info[(i+4)%8].nullnum[0]>0	)							// +010110+, +011010+
		three++;

	for(int i=0;i<4;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]==3
			&& BlackScoreBoard[m][n].info[i].nullnum[0]>0 && BlackScoreBoard[m][n].info[(i+4)%8].nullnum[0]>0	)	// +01110+
		three++;

	for(int i=0;i<8;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]+BlackScoreBoard[m][n].info[i].linknum[1]==4
			&& (BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]!=BlackScoreBoard[m][n].info[i].linknum[1])
			&& BlackScoreBoard[m][n].info[i].nullnum[0]==1	)								//排除+10111+, +11101+
		three--;

	for(int i=0;i<4;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]==3
			&& BlackScoreBoard[m][n].info[i].nullnum[0]==1 && BlackScoreBoard[m][n].info[(i+4)%8].nullnum[0]==1
			&& BlackScoreBoard[m][n].info[i].oppnum[0]>0 && BlackScoreBoard[m][n].info[(i+4)%8].oppnum[0]>0		)	//排除 2011102 （假活三）
		three--;

	return (three>1)?1:0;	
}

//44禁手判断
int Four_Ban(int m,int n){
	int four=0;
	for(int i=0;i<8;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]==4 
			&& BlackScoreBoard[m][n].info[i].nullnum[0]>0	)		// +01111+, +11110+
		four++;
	for(int i=0;i<8;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]+BlackScoreBoard[m][n].info[i].linknum[1]==4
			&& BlackScoreBoard[m][n].info[i].nullnum[0]==1	)		//+10111+, +11011+, +11101+
		four++;
	
	for(int i=0;i<4;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]+BlackScoreBoard[m][n].info[(i+4)%8].linknum[0]==4 
			&& BlackScoreBoard[m][n].info[i].nullnum[0]>0 && BlackScoreBoard[m][n].info[(i+4)%8].nullnum[0]>0 )	//排除假双四（活四）+011110+
		four--;
	return (four>1)?1:0;
}














