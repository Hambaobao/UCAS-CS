#include "head.h"

// long_connect 1
// threeban 2
// fourban 3

int Long_Connect(int ,int);
int Three_Ban(int m,int n);

int Ban_Hand(int x,int y){
	
	aRecordBoard[x][y]=1;			//填入试探棋子

	Info(aRecordBoard,x,y);

	if(Long_Connect(x,y))
		return long_connect;

//33
//44

//	aRecordBoard[x][y]=0;			//还原棋盘		

	return 0;
}

//长连禁手判断
int Long_Connect(int m,int n){
	int j=0;
	for(int i=0;i<4;i++)
		if(BlackScoreBoard[m][n].info[i].linknum[0]>5)
			j++;
	return (j>0)?1:0;	
}

//33禁手判断
int Three_Ban(int m,int n){

	return 0;	
}
