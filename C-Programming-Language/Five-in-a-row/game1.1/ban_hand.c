#include "head.h"

extern struct SingleScore MyScoreBoard[16][16];
//判断禁手
int Ban_Hand(struct SingleScore MyScoreBoard[16][16],int m,int n){	
	int a;
	for(a=0;a<4;a++)					//判断长连禁手
	if(MyScoreBoard[m][n].info[a].linknum>4)	
		return long_connect;

//	if(Three_ban(temp))
//		return threeban;
	
//	if(Four_ban(temp))
//		return fourban;



	return 0;	
}	
