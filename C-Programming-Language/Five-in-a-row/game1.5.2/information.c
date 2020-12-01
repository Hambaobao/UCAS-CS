#include "head.h"

void Info(int s[SIZE+1][SIZE+1])
{
//	s[x][y] = 1;			//填入棋子

	int temp[TSIZE][TSIZE];			//声明一个大棋盘，使其完全容纳住小棋盘
	for(int i=0;i<TSIZE;i++)
		for(int j=0;j<TSIZE;j++)
			temp[i][j]=8;		//初始化大棋盘边界，8对黑子,白子来说都相当与边界

	for(int i=1;i<=SIZE;i++)
		for(int j=1;j<=SIZE;j++)
			temp[i][j]=s[i][j];	//将小棋盘的值复制到大棋盘正中央，使其周围被厚厚的边界包裹
											
	for(int i=0;i<16;i++)			//清空数据
		for(int j=0;j<16;j++)
			for(int k=0;k<4;k++)
				for(int l=0;l<2;l++){
					BlackScoreBoard[i][j].info[k].linknum[l]=0;
					BlackScoreBoard[i][j].info[k].oppnum[l]=0;
					WhiteScoreBoard[i][j].info[k].linknum[l]=0;
					WhiteScoreBoard[i][j].info[k].oppnum[l]=0;
				}
//记录双方棋盘信息

	int m,n,i;

//记录黑棋棋盘上每一点的信息															
/*
	for(m=1;m<16;m++)
	{								//记录——向连珠数
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1][n+1+i]==1;i++)	
					BlackScoreBoard[m][n].info[0].linknum[0]++;
//				if(temp[m+1][n+1+i]==2 || temp[m+1][n+1+i]==8)	
//					BlackScoreBoard[m][n].info[0].oppnum[0]++;
//				if(temp[m+1][n+1+i+1]==1)	
//					BlackScoreBoard[m][n].info[0].linknum[1]++;
//				if(temp[m+1][n+1+i+1]==2 || temp[m+1][n+1+i+1]==8)	
//					BlackScoreBoard[m][n].info[0].oppnum[1]++;			
			}
	}

	for(m=1;m<16;m++)
	{
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1][n+1-i]==1;i++)	
					BlackScoreBoard[m][n].info[0].linknum[0]++;
//				if(temp[m+1][n+1-i]==2 || temp[m+1][n+1-i]==8)	
//					BlackScoreBoard[m][n].info[0].oppnum[0]++;
//				if(temp[m+1][n+1-i-1]==1)	
//					BlackScoreBoard[m][n].info[0].linknum[1]++;
//				if(temp[m+1][n+1-i-1]==2 || temp[m+1][n+1-i-1]==8)	
//					BlackScoreBoard[m][n].info[0].oppnum[1]++;
			}
	}

	for(m=1;m<16;m++)								//记录 / 向连珠数					
	{
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1+i][n+1+i]==1;i++)	
					BlackScoreBoard[m][n].info[1].linknum[0]++;
/*				if(temp[m+1+i][n+1+i]==2 || temp[m+1+i][n+1+i]==8)	
					BlackScoreBoard[m][n].info[1].oppnum[0]++;
				if(temp[m+1+i+1][n+1+i+1]==1)	
					BlackScoreBoard[m][n].info[1].linknum[1]++;
				if(temp[m+1+i+1][n+1+i+1]==2 || temp[m+1+i+1][n+1+i+1]==8)	
					BlackScoreBoard[m][n].info[1].oppnum[1]++;			
			}
	}
		
	for(m=1;m<16;m++)
	{
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1-i][n+1-i]==1;i++)	
					BlackScoreBoard[m][n].info[1].linknum[0]++;
/*				if(temp[m+1-i][n+1-i]==2 || temp[m+1-i][n+1-i]==8)	
					BlackScoreBoard[m][n].info[1].oppnum[0]++;
				if(temp[m+1-i-1][n+1-i-1]==1)	
					BlackScoreBoard[m][n].info[1].linknum[1]++;
				if(temp[m+1-i-1][n+1-i-1]==2 || temp[m+1-i-1][n+1-i-1]==8)	
					BlackScoreBoard[m][n].info[1].oppnum[1]++;
			}
	}

	for(m=1;m<16;m++)
	{								//记录 | 向连珠数
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1+i][n+1]==1;i++)	
					BlackScoreBoard[m][n].info[2].linknum[0]++;
/*				if(temp[m+1+i][n+1]==2 || temp[m+1+i][n+1]==8)	
					BlackScoreBoard[m][n].info[2].oppnum[0]++;
				if(temp[m+1+i+1][n+1]==1)	
					BlackScoreBoard[m][n].info[2].linknum[1]++;
				if(temp[m+1+i+1][n+1]==2 || temp[m+1+i+1][n+1]==8)	
					BlackScoreBoard[m][n].info[2].oppnum[1]++;			
			}
	}
		
	for(m=1;m<16;m++)
	{
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1-i][n+1]==1;i++)	
					BlackScoreBoard[m][n].info[2].linknum[0]++;
/*				if(temp[m+1-i][n+1]==2 || temp[m+1-i][n+1]==8)	
					BlackScoreBoard[m][n].info[2].oppnum[0]++;
				if(temp[m+1-i-1][n+1]==1)	
					BlackScoreBoard[m][n].info[2].linknum[1]++;
				if(temp[m+1-i-1][n+1]==2 || temp[m+1-i-1][n+1]==8)	
					BlackScoreBoard[m][n].info[2].oppnum[1]++;
			}
	}

	for(m=1;m<16;m++)								//记录 \ 向连珠数					
	{
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1+i][n+1-i]==1;i++)	
					BlackScoreBoard[m][n].info[0].linknum[0]++;
/*				if(temp[m+1+i][n+1-i]==2 || temp[m+1+i][n+1-i]==8)	
					BlackScoreBoard[m][n].info[0].oppnum[0]++;
				if(temp[m+1+i+1][n+1-i-1]==1)	
					BlackScoreBoard[m][n].info[0].linknum[1]++;
				if(temp[m+1+i+1][n+1-i-1]==2 || temp[m+1+i+1][n+1-i-1]==8)	
					BlackScoreBoard[m][n].info[0].oppnum[1]++;			
			}
	}
	
	for(m=1;m<16;m++)
	{	
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1-i][n+1+i]==1;i++)	
					BlackScoreBoard[m][n].info[0].linknum[0]++;
/*				if(temp[m+1-i][n+1+i]==2 || temp[m+1-i][n+1+i]==8)	
					BlackScoreBoard[m][n].info[0].oppnum[0]++;
				if(temp[m+1-i-1][n+1+i+1]==1)	
					BlackScoreBoard[m][n].info[0].linknum[1]++;
				if(temp[m+1-i-1][n+1+i+1]==2 || temp[m+1-i-1][n+1+i+1]==8)	
					BlackScoreBoard[m][n].info[0].oppnum[1]++;
			}
	}
*/
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m][n+i]==1;i++)	BlackScoreBoard[m][n].info[0].linknum[0]++;		//记录——向连珠数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m][n-i]==1;i++)	BlackScoreBoard[m][n].info[0].linknum[0]++;
							
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n+i]==1;i++)	BlackScoreBoard[m][n].info[1].linknum[0]++;		//记录 / 向连珠数	
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n-i]==1;i++)	BlackScoreBoard[m][n].info[1].linknum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n]==1;i++)	BlackScoreBoard[m][n].info[2].linknum[0]++;		//记录 | 向连珠数	
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n]==1;i++)	BlackScoreBoard[m][n].info[2].linknum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n-i]==1;i++)	BlackScoreBoard[m][n].info[3].linknum[0]++;		//记录 \ 向连珠数	
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n+i]==1;i++)	BlackScoreBoard[m][n].info[3].linknum[0]++;

	
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m][n+i]==1;i++)	;	if(temp[m][n+i]==2)	BlackScoreBoard[m][n].info[0].oppnum[0]++;		//记录——向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m][n-i]==1;i++)	;	if(temp[m][n-i]==2)	BlackScoreBoard[m][n].info[0].oppnum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n+i]==1;i++)	;	if(temp[m+i][n+i]==2)	BlackScoreBoard[m][n].info[1].oppnum[0]++;		//记录/向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n-i]==1;i++)	;	if(temp[m-i][n-i]==2)	BlackScoreBoard[m][n].info[1].oppnum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n]==1;i++)	;	if(temp[m+i][n]==2)	BlackScoreBoard[m][n].info[2].oppnum[0]++;		//记录 | 向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n]==1;i++)	;	if(temp[m-i][n]==2)	BlackScoreBoard[m][n].info[2].oppnum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n-i]==1;i++)	;	if(temp[m-i][n-i]==2)	BlackScoreBoard[m][n].info[3].oppnum[0]++;		//记录\向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n+i]==1;i++)	;	if(temp[m+i][n+i]==2)	BlackScoreBoard[m][n].info[3].oppnum[0]++;



//记录白棋棋盘上每一点的信息
/*
	for(m=1;m<16;m++)								//记录——向连珠数
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1][n+1+i]==1;i++)	
					WhiteScoreBoard[m][n].info[0].linknum[0]++;
				if(temp[m+1][n+1+i]==2 || temp[m+1][n+1+i]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[0]++;
				if(temp[m+1][n+1+i+1]==1)	
					WhiteScoreBoard[m][n].info[0].linknum[1]++;
				if(temp[m+1][n+1+i+1]==2 || temp[m+1][n+1+i+1]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[1]++;			
			}
		
	for(m=1;m<16;m++)
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1][n+1-i]==1;i++)	
					WhiteScoreBoard[m][n].info[0].linknum[0]++;
				if(temp[m+1][n+1-i]==2 || temp[m+1][n+1-i]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[0]++;
				if(temp[m+1][n+1-i-1]==1)	
					WhiteScoreBoard[m][n].info[0].linknum[1]++;
				if(temp[m+1][n+1-i-1]==2 || temp[m+1][n+1-i-1]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[1]++;
			}

	for(m=1;m<16;m++)								//记录 / 向连珠数					
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1+i][n+1+i]==1;i++)	
					WhiteScoreBoard[m][n].info[1].linknum[0]++;
				if(temp[m+1+i][n+1+i]==2 || temp[m+1+i][n+1+i]==8)	
					WhiteScoreBoard[m][n].info[1].oppnum[0]++;
				if(temp[m+1+i+1][n+1+i+1]==1)	
					WhiteScoreBoard[m][n].info[1].linknum[1]++;
				if(temp[m+1+i+1][n+1+i+1]==2 || temp[m+1+i+1][n+1+i+1]==8)	
					WhiteScoreBoard[m][n].info[1].oppnum[1]++;			
			}
		
	for(m=1;m<16;m++)
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1-i][n+1-i]==1;i++)	
					WhiteScoreBoard[m][n].info[1].linknum[0]++;
				if(temp[m+1-i][n+1-i]==2 || temp[m+1-i][n+1-i]==8)	
					WhiteScoreBoard[m][n].info[1].oppnum[0]++;
				if(temp[m+1-i-1][n+1-i-1]==1)	
					WhiteScoreBoard[m][n].info[1].linknum[1]++;
				if(temp[m+1-i-1][n+1-i-1]==2 || temp[m+1-i-1][n+1-i-1]==8)	
					WhiteScoreBoard[m][n].info[1].oppnum[1]++;
			}

	for(m=1;m<16;m++)								//记录 | 向连珠数
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1+i][n+1]==1;i++)	
					WhiteScoreBoard[m][n].info[2].linknum[0]++;
				if(temp[m+1+i][n+1]==2 || temp[m+1+i][n+1]==8)	
					WhiteScoreBoard[m][n].info[2].oppnum[0]++;
				if(temp[m+1+i+1][n+1]==1)	
					WhiteScoreBoard[m][n].info[2].linknum[1]++;
				if(temp[m+1+i+1][n+1]==2 || temp[m+1+i+1][n+1]==8)	
					WhiteScoreBoard[m][n].info[2].oppnum[1]++;			
			}
		
	for(m=1;m<16;m++)
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1-i][n+1]==1;i++)	
					WhiteScoreBoard[m][n].info[2].linknum[0]++;
				if(temp[m+1-i][n+1]==2 || temp[m+1-i][n+1]==8)	
					WhiteScoreBoard[m][n].info[2].oppnum[0]++;
				if(temp[m+1-i-1][n+1]==1)	
					WhiteScoreBoard[m][n].info[2].linknum[1]++;
				if(temp[m+1-i-1][n+1]==2 || temp[m+1-i-1][n+1]==8)	
					WhiteScoreBoard[m][n].info[2].oppnum[1]++;
			}

	for(m=1;m<16;m++)								//记录 \ 向连珠数					
		for(n=1;n<16;n++)
			{	
				for(i=0;temp[m+1+i][n+1-i]==1;i++)	
					WhiteScoreBoard[m][n].info[0].linknum[0]++;
				if(temp[m+1+i][n+1-i]==2 || temp[m+1+i][n+1-i]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[0]++;
				if(temp[m+1+i+1][n+1-i-1]==1)	
					WhiteScoreBoard[m][n].info[0].linknum[1]++;
				if(temp[m+1+i+1][n+1-i-1]==2 || temp[m+1+i+1][n+1-i-1]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[1]++;			
			}
		
	for(m=1;m<16;m++)
		for(n=1;n<16;n++)
			{
				for(i=1;temp[m+1-i][n+1+i]==1;i++)	
					WhiteScoreBoard[m][n].info[0].linknum[0]++;
				if(temp[m+1-i][n+1+i]==2 || temp[m+1-i][n+1+i]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[0]++;
				if(temp[m+1-i-1][n+1+i+1]==1)	
					WhiteScoreBoard[m][n].info[0].linknum[1]++;
				if(temp[m+1-i-1][n+1+i+1]==2 || temp[m+1-i-1][n+1+i+1]==8)	
					WhiteScoreBoard[m][n].info[0].oppnum[1]++;
			}

*/


	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m][n+i]==2;i++)	WhiteScoreBoard[m][n].info[0].linknum[0]++;		//记录——向连珠数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m][n-i]==2;i++)	WhiteScoreBoard[m][n].info[0].linknum[0]++;
										
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n+i]==2;i++)	WhiteScoreBoard[m][n].info[1].linknum[0]++;		//记录 / 向连珠数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n-i]==2;i++)	WhiteScoreBoard[m][n].info[1].linknum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n]==2;i++)	WhiteScoreBoard[m][n].info[2].linknum[0]++;		//记录 | 向连珠数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n]==2;i++)	WhiteScoreBoard[m][n].info[2].linknum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n-i]==2;i++)	WhiteScoreBoard[m][n].info[3].linknum[0]++;		//记录 \ 向连珠数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n+i]==2;i++)	WhiteScoreBoard[m][n].info[3].linknum[0]++;

	
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m][n+i]==2;i++)	;	if(temp[m][n+i]==1)	WhiteScoreBoard[m][n].info[0].oppnum[0]++;		//记录——向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m][n-i]==2;i++)	;	if(temp[m][n-i]==1)	WhiteScoreBoard[m][n].info[0].oppnum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n+i]==2;i++)	;	if(temp[m+i][n+i]==1)	WhiteScoreBoard[m][n].info[1].oppnum[0]++;		//记录/向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n-i]==2;i++)	;	if(temp[m-i][n-i]==1)	WhiteScoreBoard[m][n].info[1].oppnum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n]==2;i++)	;	if(temp[m+i][n]==1)	WhiteScoreBoard[m][n].info[2].oppnum[0]++;		//记录 | 向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n]==2;i++)	;	if(temp[m-i][n]==1)	WhiteScoreBoard[m][n].info[2].oppnum[0]++;

	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=0;temp[m+i][n-i]==2;i++)	;	if(temp[m-i][n-i]==1)	WhiteScoreBoard[m][n].info[3].oppnum[0]++;		//记录\向对手棋子数
	for(m=1;m<16;m++)	for(n=1;n<16;n++)	for(i=1;temp[m-i][n+i]==2;i++)	;	if(temp[m+i][n+i]==1)	WhiteScoreBoard[m][n].info[3].oppnum[0]++;


				
}

