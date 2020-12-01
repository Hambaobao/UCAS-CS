#include "head.h"

void Info(int s[SIZE+1][SIZE+1],int m,int n)
{
	int temp[TSIZE][TSIZE];			//声明一个大棋盘，使其完全容纳住小棋盘
	for(int i=0;i<TSIZE;i++)
		for(int j=0;j<TSIZE;j++)
			temp[i][j]=9;		//初始化大棋盘边界，9对黑子,白子来说都不能落子

	for(int i=1;i<=SIZE;i++)
		for(int j=1;j<=SIZE;j++)
			temp[i+1][j+1]=s[i][j];	//将小棋盘的值复制到大棋盘正中央，使其周围被厚厚的边界包裹
											
	for(int k=0;k<4;k++)	//清空数据
		for(int l=0;l<2;l++){
			BlackScoreBoard[m][n].info[k].linknum[l]=0;
			BlackScoreBoard[m][n].info[k].oppnum[l]=0;
			WhiteScoreBoard[m][n].info[k].linknum[l]=0;
			WhiteScoreBoard[m][n].info[k].oppnum[l]=0;
		}

//记录双方棋盘信息

		int i;

//记录黑棋棋盘上每一点的信息															


	
		for(i=0;temp[m+1][n+1+i]==1;i++)
			BlackScoreBoard[m][n].info[0].linknum[0]++;			//记录——向连珠数
		if(temp[m+1][n+1+i]==2 || temp[m+1][n+1+i]==9)	BlackScoreBoard[m][n].info[0].oppnum[0]++;	//记录——向对手棋子数
		if(temp[m+1][n+1+(i+1)]==1)	BlackScoreBoard[m][n].info[0].linknum[1]++;
		if(temp[m+1][n+1+(i+1)]==2 || temp[m+1][n+1+(i+1)]==9)	BlackScoreBoard[m][n].info[0].oppnum[1]++;		

	
		for(i=1;temp[m+1][n+1-i]==1;i++)
			BlackScoreBoard[m][n].info[0].linknum[0]++;
		if(temp[m+1][n+1-i]==2 || temp[m+1][n+1-i]==9)	BlackScoreBoard[m][n].info[0].oppnum[0]++;
		if(temp[m+1][n+1-(i+1)]==1)	BlackScoreBoard[m][n].info[0].linknum[1]++;
		if(temp[m+1][n+1-(i+1)]==2 || temp[m+1][n+1-(i+1)]==9)	BlackScoreBoard[m][n].info[0].oppnum[1]++;		
	
										
	
		for(i=0;temp[m+1+i][n+1+i]==1;i++)
			BlackScoreBoard[m][n].info[1].linknum[0]++;					//记录 / 向连珠数
		if(temp[m+1+i][n+1+i]==2 || temp[m+1+i][n+1+i]==9)	BlackScoreBoard[m][n].info[1].oppnum[0]++;		//记录/向对手棋子数
		if(temp[m+1+(i+1)][n+1+(i+1)]==1)	BlackScoreBoard[m][n].info[1].linknum[1]++;
		if(temp[m+1+(i+1)][n+1+(i+1)]==2 || temp[m+1+(i+1)][n+1+(i+1)]==9)	BlackScoreBoard[m][n].info[1].oppnum[1]++;		
	

	
		for(i=1;temp[m+1-i][n+1-i]==1;i++)
			BlackScoreBoard[m][n].info[1].linknum[0]++;
		if(temp[m+1-i][n+1-i]==2 || temp[m+1-i][n+1-i]==9)	BlackScoreBoard[m][n].info[1].oppnum[0]++;			
		if(temp[m+1-(i+1)][n+1-(i+1)]==1)	BlackScoreBoard[m][n].info[1].linknum[1]++;	
		if(temp[m+1-(i+1)][n+1-(i+1)]==2 || temp[m+1-(i+1)][n+1-(i+1)]==9)	BlackScoreBoard[m][n].info[1].oppnum[1]++;

	
		for(i=0;temp[m+1+i][n+1]==1;i++)
			BlackScoreBoard[m][n].info[2].linknum[0]++;					//记录 | 向连珠数
		if(temp[m+1+i][n+1]==2 || temp[m+1+i][n+1]==9)	BlackScoreBoard[m][n].info[2].oppnum[0]++;			//记录 | 向对手棋子数
		if(temp[m+1+(i+1)][n+1]==1)	BlackScoreBoard[m][n].info[2].linknum[1]++;
		if(temp[m+1+(i+1)][n+1]==2 || temp[m+1+(i+1)][n+1]==9)	BlackScoreBoard[m][n].info[2].oppnum[1]++;	
	
	
		for(i=1;temp[m+1-i][n+1]==1;i++)
			BlackScoreBoard[m][n].info[2].linknum[0]++;
		if(temp[m+1-i][n+1]==2 || temp[m+1-i][n+1]==9)	BlackScoreBoard[m][n].info[2].oppnum[0]++;
		if(temp[m+1-(i+1)][n+1]==1)	BlackScoreBoard[m][n].info[2].linknum[1]++;
		if(temp[m+1-(i+1)][n+1]==2 || temp[m+1-(i+1)][n+1]==9)	BlackScoreBoard[m][n].info[2].oppnum[1]++;
	

		for(i=0;temp[m+1+i][n+1-i]==1;i++)
			BlackScoreBoard[m][n].info[3].linknum[0]++;					//记录 \ 向连珠数
		if(temp[m+1+i][n+1-i]==2 || temp[m+1+i][n+1-i]==9)	BlackScoreBoard[m][n].info[3].oppnum[0]++;		//记录\向对手棋子数
		if(temp[m+1+(i+1)][n+1-(i+1)]==1)	BlackScoreBoard[m][n].info[3].linknum[1]++;
		if(temp[m+1+(i+1)][n+1-(i+1)]==2 || temp[m+1+(i+1)][n+1-(i+1)]==9)	BlackScoreBoard[m][n].info[3].oppnum[1]++;
			

		for(i=1;temp[m+1-i][n+1+i]==1;i++)
			BlackScoreBoard[m][n].info[3].linknum[0]++;
		if(temp[m+1-i][n+1+i]==2 || temp[m+1-i][n+1+i]==9)	BlackScoreBoard[m][n].info[3].oppnum[0]++;
		if(temp[m+1-(i+1)][n+1+(i+1)]==1)	BlackScoreBoard[m][n].info[3].linknum[1]++;
		if(temp[m+1-(i+1)][n+1+(i+1)]==2 || temp[m+1-(i+1)][n+1+(i+1)]==9)	BlackScoreBoard[m][n].info[3].oppnum[1]++;

			

//记录白棋棋盘上每一点的信息



		for(i=0;temp[m+1][n+1+i]==2;i++)
			WhiteScoreBoard[m][n].info[0].linknum[0]++;			//记录——向连珠数
		if(temp[m+1][n+1+i]==1 || temp[m+1][n+1+i]==9)	WhiteScoreBoard[m][n].info[0].oppnum[0]++;	//记录——向对手棋子数
		if(temp[m+1][n+1+(i+1)]==2)	WhiteScoreBoard[m][n].info[0].linknum[1]++;
		if(temp[m+1][n+1+(i+1)]==1 || temp[m+1][n+1+(i+1)]==9)	WhiteScoreBoard[m][n].info[0].oppnum[1]++;		

	
		for(i=1;temp[m+1][n+1-i]==2;i++)
			WhiteScoreBoard[m][n].info[0].linknum[0]++;
		if(temp[m+1][n+1-i]==1 || temp[m+1][n+1-i]==9)	WhiteScoreBoard[m][n].info[0].oppnum[0]++;
		if(temp[m+1][n+1-(i+1)]==2)	WhiteScoreBoard[m][n].info[0].linknum[1]++;
		if(temp[m+1][n+1-(i+1)]==1 || temp[m+1][n+1-(i+1)]==9)	WhiteScoreBoard[m][n].info[0].oppnum[1]++;		
	
										
	
		for(i=0;temp[m+1+i][n+1+i]==2;i++)
			WhiteScoreBoard[m][n].info[1].linknum[0]++;					//记录 / 向连珠数
		if(temp[m+1+i][n+1+i]==1 || temp[m+1+i][n+1+i]==9)	WhiteScoreBoard[m][n].info[1].oppnum[0]++;		//记录/向对手棋子数
		if(temp[m+1+(i+1)][n+1+(i+1)]==2)	WhiteScoreBoard[m][n].info[1].linknum[1]++;
		if(temp[m+1+(i+1)][n+1+(i+1)]==1 || temp[m+1+(i+1)][n+1+(i+1)]==9)	WhiteScoreBoard[m][n].info[1].oppnum[1]++;		
	

	
		for(i=1;temp[m+1-i][n+1-i]==2;i++)
			WhiteScoreBoard[m][n].info[1].linknum[0]++;
		if(temp[m+1-i][n+1-i]==1 || temp[m+1-i][n+1-i]==9)	WhiteScoreBoard[m][n].info[1].oppnum[0]++;			
		if(temp[m+1-(i+1)][n+1-(i+1)]==2)	WhiteScoreBoard[m][n].info[1].linknum[1]++;	
		if(temp[m+1-(i+1)][n+1-(i+1)]==1 || temp[m+1-(i+1)][n+1-(i+1)]==9)	WhiteScoreBoard[m][n].info[1].oppnum[1]++;

	
		for(i=0;temp[m+1+i][n+1]==2;i++)
			WhiteScoreBoard[m][n].info[2].linknum[0]++;					//记录 | 向连珠数
		if(temp[m+1+i][n+1]==1 || temp[m+1+i][n+1]==9)	WhiteScoreBoard[m][n].info[2].oppnum[0]++;			//记录 | 向对手棋子数
		if(temp[m+1+(i+1)][n+1]==2)	WhiteScoreBoard[m][n].info[2].linknum[1]++;
		if(temp[m+1+(i+1)][n+1]==1 || temp[m+1+(i+1)][n+1]==9)	WhiteScoreBoard[m][n].info[2].oppnum[1]++;	
	
	
		for(i=1;temp[m+1-i][n+1]==2;i++)
			WhiteScoreBoard[m][n].info[2].linknum[0]++;
		if(temp[m+1-i][n+1]==1 || temp[m+1-i][n+1]==9)	WhiteScoreBoard[m][n].info[2].oppnum[0]++;
		if(temp[m+1-(i+1)][n+1]==2)	WhiteScoreBoard[m][n].info[2].linknum[1]++;
		if(temp[m+1-(i+1)][n+1]==1 || temp[m+1-(i+1)][n+1]==9)	WhiteScoreBoard[m][n].info[2].oppnum[1]++;
	

		for(i=0;temp[m+1+i][n+1-i]==2;i++)
			WhiteScoreBoard[m][n].info[3].linknum[0]++;					//记录 \ 向连珠数
		if(temp[m+1+i][n+1-i]==1 || temp[m+1+i][n+1-i]==9)	WhiteScoreBoard[m][n].info[3].oppnum[0]++;		//记录\向对手棋子数
		if(temp[m+1+(i+1)][n+1-(i+1)]==2)	WhiteScoreBoard[m][n].info[3].linknum[1]++;
		if(temp[m+1+(i+1)][n+1-(i+1)]==1 || temp[m+1+(i+1)][n+1-(i+1)]==9)	WhiteScoreBoard[m][n].info[3].oppnum[1]++;
			

		for(i=1;temp[m+1-i][n+1+i]==2;i++)
			WhiteScoreBoard[m][n].info[3].linknum[0]++;
		if(temp[m+1-i][n+1+i]==1 || temp[m+1-i][n+1+i]==9)	WhiteScoreBoard[m][n].info[3].oppnum[0]++;
		if(temp[m+1-(i+1)][n+1+(i+1)]==2)	WhiteScoreBoard[m][n].info[3].linknum[1]++;
		if(temp[m+1-(i+1)][n+1+(i+1)]==1 || temp[m+1-(i+1)][n+1+(i+1)]==9)	WhiteScoreBoard[m][n].info[3].oppnum[1]++;



				
}







