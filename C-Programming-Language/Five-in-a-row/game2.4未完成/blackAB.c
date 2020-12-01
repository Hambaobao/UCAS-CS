#include "head.h"

struct point black_ai_minmax_alphabeta(void){
	int min_alphabeta(int ,struct point);
	struct zl;
	int x,y,k;
	int tmp=0;
	int best= -20000;

	generate_black_point();

	k=0;
	for(int i=0;i<black_quene.len;i++)		//一个一个子轮着试一遍
	{
		x=black_quene.chess[i].x;
		y=black_quene.chess[i].y;
		aRecordBoard[x][y]=1;			//尝试落子

		tmp=min_alphabeta(depth-1,black_quene.chess[i]);

		if(tmp==best)				//找到和当前最高分一样的分数，保存下来;
		{
			black_sure_quene.chess[k].x=black_quene.chess[i].x;
			black_sure_quene.chess[k].y=black_quene.chess[i].y;
			black_sure_quene.len=k+1;
			k++;	
		}

		if(tmp>best)				//找到更好的分
		{
			best=tmp;
			k=0;
			black_sure_quene.len=1;//清空最优队列
			black_sure_quene.chess[k].x=black_quene.chess[i].x;
			black_sure_quene.chess[k].y=black_quene.chess[i].y;
		}
		
		aRecordBoard[x][y]=0;			//还原棋盘
			
	}

	k=(int)(rand()%black_sure_quene.len);		//如果有多个最高分点，随机选一个下
	zl.x=black_sure_quene.chess[k].x;
	zl.y=black_sure_quene.chess[k].y;

	return zl;
}

//找到得分最高的那些点,存到black_quene中
void generate_black_point(void){
	int max=0;
																//先将记录最高分位置的数组清零
	for(int i=0;i<225;i++)
		{
			black_quene.chess[i].x=0;
			black_quene.chess[i].y=0;	
		}
	black_quene.len=0;

	arrange_grades_black();
																//获得合成后的分数
	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			balck_score_board[i][j]=BlackTable.Table[0][i][j].score+BlackTable.Table[1][i][j].score
						+BlackTable.Table[2][i][j].score+BlackTable.Table[3][i][j].score;
																//找到最高得分
	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(max<balck_score_board[i][j])
			{
				max=balck_score_board[i][j];
			}
																//得到分数最高的点
	int k=0;
	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			if(max==balck_score_board[i][j])
			{
				black_quene.chess[k].x=i;
				black_quene.chess[k].y=j;
				black_quene.len=k+1;
				k++;	
			}
}

int min_alphabeta(int deep,struct point chess){
	int res;
	res=get_grades_white(0,chess.x,chess.y)+get_grades_white(1,chess.x,chess.y)
		+get_grades_white(2,chess.x,chess.y)+get_grades_white(3,chess.x,chess.y);
	

}


















