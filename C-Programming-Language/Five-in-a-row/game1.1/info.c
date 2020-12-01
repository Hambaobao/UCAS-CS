#include "head.h"
struct SingleScore MyScoreBoard[16][16];		//记录棋盘上每一点的得分

void Info(int s[SIZE+1][SIZE+1])
{

	int temp[TSIZE][TSIZE];			//声明一个大棋盘，使其完全容纳住小棋盘
	for(int i=0;i<TSIZE;i++)
		for(int j=0;j<TSIZE;j++)
			temp[i][j]=2;		//初始化大棋盘边界，对黑子来说，2是白子也是棋盘外部，均不可落子

	for(int i=1;i<=SIZE;i++)
		for(int j=1;j<=SIZE;j++)
			temp[i][j]=s[i][j];	//将小棋盘的值复制到大棋盘正中央，使其周围被厚厚的边界包裹
											
	for(int i=0;i<16;i++)			//清空数据
		for(int j=0;j<16;j++)
			for(int k=0;k<4;k++){
				MyScoreBoard[i][j].info[k].linknum=0;
				MyScoreBoard[i][j].info[k].oppnum=0;
			}

//记录棋盘上每一点的信息

for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m][n+i]==1;i++)	MyScoreBoard[m][n].info[0].linknum++;		//记录——向连珠数
for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m][n-i]==1;i++)	MyScoreBoard[m][n].info[0].linknum++;
										
for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m+i][n+i]==1;i++)	MyScoreBoard[m][n].info[1].linknum++;		//记录 / 向连珠数
for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m-i][n-i]==1;i++)	MyScoreBoard[m][n].info[1].linknum++;

for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m+i][n]==1;i++)	MyScoreBoard[m][n].info[2].linknum++;		//记录 | 向连珠数
for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m-i][n]==1;i++)	MyScoreBoard[m][n].info[2].linknum++;

for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m-i][n-i]==1;i++)	MyScoreBoard[m][n].info[3].linknum++;		//记录 \ 向连珠数
for(int m=0;m<16;m++)	for(int n=0;n<16;n++)	for(int i=1;temp[m+i][n+i]==1;i++)	MyScoreBoard[m][n].info[3].linknum++;

int m,n,i;

for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m][n+i]==1;i++)	;	if(temp[m+i][n]==2)	MyScoreBoard[m][n].info[0].oppnum++;		//记录——向对手棋子数
for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m-i][n-i]==1;i++)	;	if(temp[m-i][n]==2)	MyScoreBoard[m][n].info[0].oppnum++;

for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m+i][n+i]==1;i++)	;	if(temp[m+i][n+i]==2)	MyScoreBoard[m][n].info[1].oppnum++;		//记录/向对手棋子数
for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m-i][n-i]==1;i++)	;	if(temp[m-i][n-i]==2)	MyScoreBoard[m][n].info[1].oppnum++;

for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m+i][n]==1;i++)	;	if(temp[m][n+i]==2)	MyScoreBoard[m][n].info[2].oppnum++;		//记录 | 向对手棋子数
for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m-i][n]==1;i++)	;	if(temp[m][n-i]==2)	MyScoreBoard[m][n].info[2].oppnum++;

for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m-i][n-i]==1;i++)	;	if(temp[m-i][n+i]==2)	MyScoreBoard[m][n].info[3].oppnum++;		//记录\向对手棋子数
for(m=0;m<16;m++)	for(n=0;n<16;n++)	for(i=1;temp[m+i][n+i]==1;i++)	;	if(temp[m+i][n-i]==2)	MyScoreBoard[m][n].info[3].oppnum++;
	
		
}


