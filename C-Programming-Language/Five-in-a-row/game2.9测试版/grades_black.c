#include "head.h"


struct ScoreTable arrange_grades_black(void){
	long long int get_grades_black(int ,int);

	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			BlackTable.Table[i][j].score=get_grades_black(i,j);
	return BlackTable;
				
}


long long int get_grades_black(int x,int y){											//获取最后分数
	long long int get_my_grades_black(int ,int ,int);
	long long int get_opp_grades_black(int ,int ,int);

	long long int mygrades=0;
	long long int oppgrades=0;

	mygrades=get_my_grades_black(0,x,y)+get_my_grades_black(1,x,y)+get_my_grades_black(2,x,y)+get_my_grades_black(3,x,y);
	oppgrades=get_opp_grades_black(0,x,y)+get_opp_grades_black(1,x,y)+get_opp_grades_black(2,x,y)+get_opp_grades_black(3,x,y);

	return mygrades+oppgrades;

}


long long int get_my_grades_black(int i,int x,int y){
//	int whitedeadpiece(int ,int);
	int s[16][16],t;

	if(IsChessPiece(x,y))												//有棋子赋值为0
		return 0;														
	

	if(Ban_Hand(x,y)>0)												//黑棋禁手
		return 0;


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			s[i][j]=aRecordBoard[i][j];									//复制当前棋局

	s[x][y]=1;													//填入试探棋子
	
	Info(s,x,y);													//获取落子处棋盘信息
		



	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==5)			//黑棋连5
		return 549755813888;
/*
	if(deadpiece(x,y)==4)												//4个方向死棋
		return 40;

	if(deadpiece(x,y)==3)												//3个方向死棋
		return 100;
*/


	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==4			//黑棋连活4
	&& (BlackScoreBoard[x][y].info[i].nullnum[0]>0 && BlackScoreBoard[x][y].info[i+4].nullnum[0]>0))		
		return 8589934592;


	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==3			//黑棋连活3
	&& (BlackScoreBoard[x][y].info[i].nullnum[0]>0 && BlackScoreBoard[x][y].info[i+4].nullnum[0]>0)
	&& !(BlackScoreBoard[x][y].info[i].nullnum[0]==1 && BlackScoreBoard[x][y].info[(i+4)%8].nullnum[0]==1
	&& BlackScoreBoard[x][y].info[i].oppnum[0]>0 && BlackScoreBoard[x][y].info[(i+4)%8].oppnum[0]>0))		
		return 134217728;


	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==2 
	&& BlackScoreBoard[x][y].info[i].nullnum[0]>0 && BlackScoreBoard[x][y].info[i+4].nullnum[0]>0			//黑棋连活2
	&& !(BlackScoreBoard[x][y].info[i].nullnum[0]+BlackScoreBoard[x][y].info[(i+4)%8].nullnum[0]==3
	&& BlackScoreBoard[x][y].info[i].oppnum[0]>0 && BlackScoreBoard[x][y].info[(i+4)%8].oppnum[0]>0))			
		return 2097152;



	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==4 
	&& (BlackScoreBoard[x][y].info[i].oppnum[0]>0 ^ BlackScoreBoard[x][y].info[i+4].oppnum[0]>0))			//黑棋眠4
		return 32768;

/*	
	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[(i+4)%8].linknum[0]
	+BlackScoreBoard[x][y].info[i].linknum[1]==4 && BlackScoreBoard[x][y].info[i].nullnum[0]==1)			//黑棋跳活4
		return 590490;
*/


	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==3
	&& (BlackScoreBoard[x][y].info[i].oppnum[0]>0 ^ BlackScoreBoard[x][y].info[i+4].linknum[0]>0))			//黑棋眠3
		return 512;

/*
	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[(i+4)%8].linknum[0]
	+BlackScoreBoard[x][y].info[i].linknum[1]==3 && BlackScoreBoard[x][y].info[i].nullnum[0]==1 
	&& BlackScoreBoard[x][y].info[i].nullnum[1]>0 && BlackScoreBoard[x][y].info[(i+4)%8].nullnum[0]>0)		//黑棋跳活3
		return 7290;
*/			

	
	if(BlackScoreBoard[x][y].info[i].linknum[0]+BlackScoreBoard[x][y].info[i+4].linknum[0]==1 
	&& BlackScoreBoard[x][y].info[i].nullnum[0]>0 && BlackScoreBoard[x][y].info[i+4].nullnum[0]>0
	&& !(BlackScoreBoard[x][y].info[i].nullnum[0]+BlackScoreBoard[x][y].info[(i+4)%8].nullnum[0]==4
	&& BlackScoreBoard[x][y].info[i].oppnum[0]>0 && BlackScoreBoard[x][y].info[(i+4)%8].oppnum[0]>0))			//黑棋活1
		return 8;	
															
	return 40;													//其他情况
}

long long int get_opp_grades_black(int i,int x,int y){

//	int whitedeadpiece(int ,int);
	int s[16][16];
	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			s[i][j]=aRecordBoard[i][j];									//复制当前棋局

	if(IsChessPiece(x,y))												//有棋子赋值为0
		return 0;
	
	s[x][y]=2;													//填入试探棋子

	Info(s,x,y);													//获取落子处棋盘信息
	
	
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]>4)			//白棋连5
		return 68719476736;
	
	
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]==4			//白棋连活4
	&& WhiteScoreBoard[x][y].info[i].nullnum[0]>0 && WhiteScoreBoard[x][y].info[i+4].nullnum[0]>0)		
		return 1073741824;

	
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]==3			//白棋连活3
	&& WhiteScoreBoard[x][y].info[i].nullnum[0]>0 && WhiteScoreBoard[x][y].info[i+4].nullnum[0]>0
	&& !(WhiteScoreBoard[x][y].info[i].nullnum[0]==1 && WhiteScoreBoard[x][y].info[(i+4)%8].nullnum[0]==1
	&& WhiteScoreBoard[x][y].info[i].oppnum[0]>0 && WhiteScoreBoard[x][y].info[(i+4)%8].oppnum[0]>0))
		return 16777216;

	
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]==4 
	&& (WhiteScoreBoard[x][y].info[i].oppnum[0]>0 ^ WhiteScoreBoard[x][y].info[i+4].oppnum[0]>0))			//白棋眠4
		return 262144;

/*
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[(i+4)%8].linknum[0]
	+WhiteScoreBoard[x][y].info[i].linknum[1]==4 && WhiteScoreBoard[x][y].info[i].nullnum[0]==1)			//白棋跳活4
		return 1771470;	
*/


	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]==3
	&& (WhiteScoreBoard[x][y].info[i].oppnum[0]>0 ^ WhiteScoreBoard[x][y].info[i+4].linknum[0]>0))			//白棋眠3
		return 4096;

/*
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[(i+4)%8].linknum[0]
	+WhiteScoreBoard[x][y].info[i].linknum[1]==3 && WhiteScoreBoard[x][y].info[i].nullnum[0]==1 
	&& WhiteScoreBoard[x][y].info[i].nullnum[1]>0 && WhiteScoreBoard[x][y].info[(i+4)%8].nullnum[0]>0)		//白棋跳活3
		return 21870;
*/


	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]==2			//白棋连活2
	&& (WhiteScoreBoard[x][y].info[i].nullnum[0]>0 && WhiteScoreBoard[x][y].info[i+4].nullnum[0]>0)
	&& !(WhiteScoreBoard[x][y].info[i].nullnum[0]+WhiteScoreBoard[x][y].info[(i+4)%8].nullnum[0]==3
	&& WhiteScoreBoard[x][y].info[i].oppnum[0]>0 && WhiteScoreBoard[x][y].info[(i+4)%8].oppnum[0]>0))
		return 64;

		
	if(WhiteScoreBoard[x][y].info[i].linknum[0]+WhiteScoreBoard[x][y].info[i+4].linknum[0]==1 
	&& WhiteScoreBoard[x][y].info[i].nullnum[0]>0 && WhiteScoreBoard[x][y].info[i+4].nullnum[0]>0
	&& !(WhiteScoreBoard[x][y].info[i].nullnum[0]+WhiteScoreBoard[x][y].info[(i+4)%8].nullnum[0]==4
	&& WhiteScoreBoard[x][y].info[i].oppnum[0]>0 && WhiteScoreBoard[x][y].info[(i+4)%8].oppnum[0]>0))		//白棋活1
		return 8;	

															
	return 40;													//其他情况

}

/*															//判断几个方向死棋
int blackdeadpiece(int x,int y){
	int b=0;
	for(int i=0;i<4;i++)
	if(BlackScoreBoard[x][y].info[i].nullnum[0]==1 && BlackScoreBoard[x][y].info[i+4].nullnum[0]==1))		
		b++;
	return b;				
}


*/









