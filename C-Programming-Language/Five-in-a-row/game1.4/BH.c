#include <stdio.h>
#include <stdlib.h>

int Broad[15][15];
int Record[225][3];
int player = 1;
int game = 0;
int exitvalue = 0;
int notecode = 0;
int Style_Me;
int Style_Foe;
int SetDepth;
int DepthValue[7];


//根据数组的值来显示棋盘
//只需要Broad数组
void Broad_Display()
{
	int i, j, k;
	k = 15;

	system("color F0");
	system("cls");

	for (i = 0; i <= 14; i++)
	{
		printf("%2d", k);
		k--;

		for (j = 0; j <= 14; j++)
		{
			switch (Broad[i][j])
			{
			case 1:
				printf("┏");
				break;

			case 2:
				printf("┓");
				break;

			case 3:
				printf("┛");
				break;

			case 4:
				printf("┗");
				break;

			case 5:
				printf("┠");
				break;

			case 6:
				printf("┯");
				break;

			case 7:
				printf("┨");
				break;

			case 8:
				printf("┷");
				break;

			case 9:
				printf("┼");
				break;

			case 10:
				printf("╋");
				break;

			case 11:
				printf("●");
				break;

			case 12:
				printf("▲");
				break;

			case 21:
				printf("◎");
				break;

			case 22:
				printf("△");
				break;
			}
			if (j == 14)
			{
				printf("\n");
			}
		}
	}

	printf("   A B C D E F G H I J K L M N O\n");
}

//把数组所有值归零
//需要Broad，player
void Broad_Initialize()
{
	int i, j;

	for (i = 0; i <= 14; i++)
	{
		for (j = 0; j <= 14; j++)
		{
			Broad[i][j] = 0;
		}
	}

	player = 1;
	game = 0;
}

//给数组中为0的值赋值
//只需要Broad数组
void Broad_State()
{
	int i, j;

	for (i = 0; i <= 14; i++)
	{
		for (j = 0; j <= 14; j++)
		{
			if (Broad[i][j] == 0)
			{
				if (i == 0) //第一行
				{
					if (j == 0)
						Broad[i][j] = 1;
					else if (j == 14)
						Broad[i][j] = 2;
					else
						Broad[i][j] = 6;
				}
				else if (i == 14) //最后一行
				{
					if (j == 0)
						Broad[i][j] = 4;
					else if (j == 14)
						Broad[i][j] = 3;
					else
						Broad[i][j] = 8;
				}
				else if (j == 0)
					Broad[i][j] = 5; //左边缘
				else if (j == 14)
					Broad[i][j] = 7; //右边缘
				else if ((i == 7 && j == 7) || ((i == 3 || i == 11) && (j == 3 || j == 11)))
					Broad[i][j] = 10; //“天元”和“星”
				else
					Broad[i][j] = 9; //其他点
			}
		}
	}
}


//根据代码显示提示
//需要notecode，game，Broad初始化函数
void Display_Note()
{
	printf("\n");

	switch (notecode)
	{
	case 0:
		break;

	case 1:
		printf("玩家1（黑棋）胜利!\n");
		printf("按R或r重开\n\n");
		break;

	case 2:
		printf("玩家2（白棋）胜利!\n\n");
		printf("按R或r重开\n\n");
		break;

	case 3:
		printf("禁手，请重下\n");
		break;

	case 4:
		printf("禁手，请重下\n");
		break;

	case 5:
		printf("超出棋盘范围咯，请重下\n");
		break;

	case 6:
		printf("只有小朋友才会想把棋落在已经下了的棋上面哦~\n");
		break;

	case 7:
		printf("不合法的输入，请重新输入\n");
		break;

	case 8:
		printf("无法撤销了哦\n");
		break;

	case 9:
		printf("夜深了早点休息吧o(*￣▽￣*)ブ晚安好梦\n\n");
		break;

	case 110:
		printf("BUG\n");
		break;
	}

	if (game >= 225)
	{
		Broad_Initialize();
		printf("平局!\n\n");
	}

	if (game == 0)
	{
		printf("================================\n");
		printf("假装可以用的五子棋\n");
		printf("输入U或u撤销，输入R或r重新开始，输入Q或q退出\n");
		printf("================================\n");
	}

	notecode = 0;
}

//根据目前的玩家状态在i，j点落子，然后切换玩家
//需要Broad数组，Record数组，game，player，notecode
void down(int i, int j)
{
	int k;

	if (Broad[i][j] < 11)
	{
		++game;

		for (k = (game - 1); k > 0; k--)
		{
			Record[k][0] = Record[k - 1][0];
			Record[k][1] = Record[k - 1][1];
			Record[k][2] = Record[k - 1][2];
		}

		Record[0][0] = player;
		Record[0][1] = i;
		Record[0][2] = j;

		Broad[i][j] = player * 10 + 2;

		if (game >= 3)
			(Broad[Record[2][1]][Record[2][2]])--;

		player = 3 - player;
	}
	else
		notecode = 6;
}

//返回上一步的状态，并且将玩家切换回去
//需要Broad数组，Record数组，game，notecode
void undo()
{
	int k;

	if (game >= 1)
	{
		player = Record[0][0];
		Broad[Record[0][1]][Record[0][2]] = 0;

		if (game >= 3)
			(Broad[Record[2][1]][Record[2][2]])++; //若前一步存在，将之前的复原

		for (k = 0; k < (game - 1); k++)
		{
			Record[k][0] = Record[k + 1][0];
			Record[k][1] = Record[k + 1][1];
			Record[k][2] = Record[k + 1][2];
		}
		--game;
	}
	else
		notecode = 8;
}


//辅助函数：通过棋盘状态输出成一个数
//需要Broad数组
void getstate(int i, int j, int* number)
{
	int temp;

	if (i >= 0 && i <= 14 && j >= 0 && j <= 14)
	{
		switch (Broad[i][j])
		{
		case 11:
			temp = 1;
			break;

		case 12:
			temp = 1;
			break;

		case 21:
			temp = 2;
			break;

		case 22:
			temp = 2;
			break;

		default:
			temp = 0;
			break;
		}
	}
	else
		temp = 3;

	*number = 4 * (*number) + temp;
}

//输出横线的附近几个值，用于判断禁手
//只需要getstate函数
int near_1(int i, int j)
{
	int k;
	int number = 0;
	j = j - 5;

	for (k = 0; k <= 10; k++)
	{
		getstate(i, j, &number);

		j++;
	}

	return number;
}

//输出竖线的附近几个值，用于判断禁手
//只需要getstate函数
int near_2(int i, int j)
{
	int k;
	int number = 0;
	i = i - 5;

	for (k = 0; k <= 10; k++)
	{
		getstate(i, j, &number);

		i++;
	}

	return number;
}

//输出左上右下斜线的附近几个值，用于判断禁手
//只需要getstate函数
int near_3(int i, int j)
{
	int k;
	int number = 0;
	i = i - 5;
	j = j - 5;

	for (k = 0; k <= 10; k++)
	{
		getstate(i, j, &number);

		i++;
		j++;
	}

	return number;
}

//输出左上右下斜线的附近几个值，用于判断禁手
//只需要getstate函数
int near_4(int i, int j)
{
	int k;
	int number = 0;
	i = i + 5;
	j = j - 5;

	for (k = 0; k <= 10; k++)
	{
		getstate(i, j, &number);

		i--;
		j++;
	}

	return number;
}

//通过读数判断是否胜利，同时读取活四，对黑白均有效
//什么都不需要
void Cal_Win(int number, int* five, int* livefour, int player)
{
	int line[11];
	int t;

	for (t = 10; t >= 0; t--)
	{
		line[t] = number % 4;
		number = (number - (number % 4)) / 4;
	}

	if (player == 1)
	{
		for (t = 0; t <= 4; t++)
		{
			if (line[t + 0] != player && line[t + 1] == player && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player && line[t + 6] != player)
				(*five)++;
		}
	}
	else
	{
		for (t = 0; t <= 4; t++)
		{
			if (line[t + 1] == player && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player)
				(*five)++;
		}
	}

	for (t = 0; t <= 3; t++)
	{
		if (line[t + 0] != player && line[t + 1] == 0 && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player && line[t + 6] == 0 && line[t + 7] != player)
			(*livefour)++; //+011110+
	}

}

//通过读数判断是否是禁手，对黑白均有效
//什么都不需要
void Cal_Ban(int number, int* three, int* four, int* more, int player)
{
	int line[11];
	int t;

	for (t = 10; t >= 0; t--)
	{
		line[t] = number % 4;
		number = (number - (number % 4)) / 4;
	}

	//判断一系列活3，自动判断双活3
	for (t = 0; t <= 3; t++)
	{
		if (line[t + 0] != player && line[t + 1] == 0 && line[t + 2] == player)
		{
			if (line[t + 3] == 0 && line[t + 4] == player && line[t + 5] == player && line[t + 6] == 0 && line[t + 7] != player)
				(*three)++; //+010110+
			if (line[t + 3] == player && line[t + 4] == 0 && line[t + 5] == player && line[t + 6] == 0 && line[t + 7] != player)
				(*three)++; //+011010+
			if (line[t + 3] == player && line[t + 4] == player && line[t + 5] == 0 && line[t + 6] != player)
				(*three)++; //+01110+
		}
	}

	//排除假活3
	for (t = 1; t <= 2; t++)
	{
		if ((line[0] == (3 - player) || line[0] == 3) && line[1] == 0 && line[2] == player && line[3] == player && line[4] == player && line[5] == 0 && (line[6] == (3 - player) || line[6] == 3))
			(*three)--; //2011102
	}

	//判断一系列四，自动判断双四
	for (t = 0; t <= 4; t++)
	{
		if (line[t + 0] != player && line[t + 6] != player)
		{
			if (line[t + 1] == 0 && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player)
				(*four)++; //+01111+
			if (line[t + 1] == player && line[t + 2] == 0 && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player)
				(*four)++; //+10111+
			if (line[t + 1] == player && line[t + 2] == player && line[t + 3] == 0 && line[t + 4] == player && line[t + 5] == player)
				(*four)++; //+11011+
			if (line[t + 1] == player && line[t + 2] == player && line[t + 3] == player && line[t + 4] == 0 && line[t + 5] == player)
				(*four)++; //+11101+
			if (line[t + 1] == player && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == 0)
				(*four)++; //+11110+
		}
	}

	//排除假双四（活四）
	for (t = 0; t <= 3; t++)
	{
		if (line[t + 0] != player && line[t + 1] == 0 && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player && line[t + 6] == 0 && line[t + 7] != player)
			(*four)--; //+011110+
	}

	//判断长连
	for (t = 0; t <= 5; t++)
	{
		if (line[t + 0] == player && line[t + 1] == player && line[t + 2] == player && line[t + 3] == player && line[t + 4] == player && line[t + 5] == player)
			(*more)++;
	}
}

//通过读数判断附近子的距离，仅限几条线上相连的
//什么都不需要
void Cal_Distance(int number, int* distance)
{
	int line[11];
	int t;

	for (t = 10; t >= 0; t--)
	{
		line[t] = number % 4;
		number = (number - (number % 4)) / 4;
	}

	for (t = 0; t <= 5; t++)
	{
		if (line[5 - t] == 1 || line[5 - t] == 2 || line[5 + t] == 1 || line[5 + t] == 2)
		{
			if ((*distance) > t)
				(*distance) = t;
			return;
		}
	}

	if ((*distance) > t)
		(*distance) = t;
	return;
}

//通过读数判断是否有连着的二
//什么都不需要
void Cal_Two(int number, int* two_4, int* two_3, int* two_2, int* two_1, int player)
{
	int line[11];
	int t;

	for (t = 10; t >= 0; t--)
	{
		line[t] = number % 4;
		number = (number - (number % 4)) / 4;
	}

	//判断四个空的2
	for (t = 0; t <= 1; t++)
	{
		if (line[t + 0] == 0 && line[t + 1] == 0 && line[t + 2] == 0 && line[t + 3] == 0 && line[t + 4] == 1 && line[t + 5] == 1 && line[t + 6] == 0 && line[t + 7] == 0 && line[t + 8] == 0 && line[t + 9] == 0)
		{
			(*two_4)++; //0000110000
			return;
		}
	}

	//判断三个空的2
	for (t = 0; t <= 1; t++)
	{
		if (line[t + 1] == 0 && line[t + 2] == 0 && line[t + 3] == 0 && line[t + 4] == 1 && line[t + 5] == 1 && line[t + 6] == 0 && line[t + 7] == 0 && line[t + 8] == 0)
		{
			(*two_3)++; //00011000
			return;
		}
	}

	//判断两个空的2
	for (t = 0; t <= 1; t++)
	{
		if (line[t + 2] == 0 && line[t + 3] == 0 && line[t + 4] == 1 && line[t + 5] == 1 && line[t + 6] == 0 && line[t + 7] == 0)
		{
			(*two_2)++; //001100
			return;
		}
	}

	//判断一个空的2
	for (t = 0; t <= 1; t++)
	{
		if (line[t + 3] == 0 && line[t + 4] == 1 && line[t + 5] == 1 && line[t + 6] == 0)
		{
			(*two_1)++; //001100
			return;
		}
	}
}

//判断玩家然后自动判断禁手，自动撤销和修改note
//需要Record数组，notecode，Cal_Ban函数，undo函数，一系列near函数
void banjudge()
{
	if (Record[0][0] == 1)
	{
		int three = 0;
		int four = 0;
		int more = 0;

		Cal_Ban(near_1(Record[0][1], Record[0][2]), &three, &four, &more, 1);
		Cal_Ban(near_2(Record[0][1], Record[0][2]), &three, &four, &more, 1);
		Cal_Ban(near_3(Record[0][1], Record[0][2]), &three, &four, &more, 1);
		Cal_Ban(near_4(Record[0][1], Record[0][2]), &three, &four, &more, 1);

		if (three >= 2 || four >= 2 || more > 0)
		{
			undo();
			notecode = 3;
		}
	}
}

//判断胜利，自动修改note
//需要Record数组，notecode，Cal_Win函数，一系列near函数
void winjudge()
{
	int five = 0;
	int livefour = 0;

	Cal_Win(near_1(Record[0][1], Record[0][2]), &five, &livefour, Record[0][0]);
	Cal_Win(near_2(Record[0][1], Record[0][2]), &five, &livefour, Record[0][0]);
	Cal_Win(near_3(Record[0][1], Record[0][2]), &five, &livefour, Record[0][0]);
	Cal_Win(near_4(Record[0][1], Record[0][2]), &five, &livefour, Record[0][0]);

	if (five > 0)
		notecode = Record[0][0];
}

//判断胜利和禁手，黑子若已五连则不判断禁手
//需要notecode，winjudge函数，banjudge函数
void Judge()
{
	if (game >= 1)
	{
		winjudge();
		if (notecode != 1 && notecode != 2)
		{
			banjudge();
		}
	}
}

int ScoreTable_1[15][15];
int ScoreTable_2[15][15];

//设置style参数，styleme/stylefoe比值越高，攻击性越强
void AI_Score_Style()
{
	Style_Me = 2;
	Style_Foe = 1;
}

//设置深度，但根据实际测量发现，虽然可以运行，但是算法有问题，因此设为0
void AI_Score_SetDepth()
{
	SetDepth = 0;
	DepthValue[0] = 20;
	DepthValue[1] = 6;
	DepthValue[2] = 5;
	DepthValue[3] = 3;
	DepthValue[4] = 3;
	DepthValue[5] = 2;
	DepthValue[6] = 1;
}

//设置不同深度分数权重
int AI_Score_Depth(int depth)
{
	if ((depth / 2) == 0)
		return DepthValue[0];
	else if ((depth / 2) == 1)
		return DepthValue[1];
	else if ((depth / 2) == 2)
		return DepthValue[2];
	else if ((depth / 2) == 3)
		return DepthValue[3];
	else if ((depth / 2) == 4)
		return DepthValue[4];
	else if ((depth / 2) == 5)
		return DepthValue[5];
	else
		return DepthValue[6];
}

//辅助变量，结构体，附近棋局数据
struct neighbour {
	int near_1;
	int near_2;
	int near_3;
	int near_4;
};

//辅助函数，搜索附近棋局数据
struct neighbour scan_neighbour(int i, int j)
{
	struct neighbour neighbour;

	neighbour.near_1 = near_1(i, j);
	neighbour.near_2 = near_2(i, j);
	neighbour.near_3 = near_3(i, j);
	neighbour.near_4 = near_4(i, j);

	return neighbour;
}

//根据点和player，判断是否是禁手，禁手则输出-1，可行则输出0
//需要Broad数组，Cal_Ban函数，Cal_Win函数，一系列near函数
int AI_Score_Ban(int i, int j, int playerin)
{
	if (playerin == 2)
		return 0;
	else
	{
		int three = 0;
		int four = 0;
		int five = 0;
		int livefour = 0;
		int more = 0;
		int originalstate = Broad[i][j];

		Broad[i][j] = 11; //由于确定了是黑棋，直接用11代入

		Cal_Ban(near_1(i, j), &three, &four, &more, playerin);
		Cal_Ban(near_2(i, j), &three, &four, &more, playerin);
		Cal_Ban(near_3(i, j), &three, &four, &more, playerin);
		Cal_Ban(near_4(i, j), &three, &four, &more, playerin);
		Cal_Win(near_1(i, j), &five, &livefour, playerin);
		Cal_Win(near_2(i, j), &five, &livefour, playerin);
		Cal_Win(near_3(i, j), &five, &livefour, playerin);
		Cal_Win(near_4(i, j), &five, &livefour, playerin);

		Broad[i][j] = originalstate;

		if (five == 0)
			if (three >= 2 || four >= 2 || more > 0)
				return -1;
	}

	return 0;
}

//根据点判断最近距离，并输出基本分，重叠时输出-1
//需要Cal_Distance函数，一系列near函数
int AI_Score_Distance(int i, int j)
{
	//计算距离
	int distance = 6;
	Cal_Distance(near_1(i, j), &distance);
	Cal_Distance(near_2(i, j), &distance);
	Cal_Distance(near_3(i, j), &distance);
	Cal_Distance(near_4(i, j), &distance);

	//根据距离打分
	switch (distance)
	{
	case 0:
		return -1;
		break;

	case 1:
		return 4;
		break;

	case 2:
		return 3;
		break;

	case 3:
		return 2;
		break;

	case 4:
		return 0;
		break;

	case 5:
		return 0;
		break;

	case 6:
		return 0;
		break;
	}

	notecode = 110;
	return 0;
}

//根据点和player，自动更换棋子，估分，没有同时考虑禁手与否
int AI_Score_Basic(int i, int j, int playerin)
{
	int basicscore = 0;
	struct neighbour neighbour;
	int line[4][11];
	int k, t;
	int originalstate = Broad[i][j];

	Broad[i][j] = 10 * playerin + 1;

	neighbour = scan_neighbour(i, j);

	for (t = 10; t >= 0; t--)
	{
		line[0][t] = neighbour.near_1 % 4;
		neighbour.near_1 = (neighbour.near_1 - (neighbour.near_1 % 4)) / 4;

		line[1][t] = neighbour.near_2 % 4;
		neighbour.near_2 = (neighbour.near_2 - (neighbour.near_2 % 4)) / 4;

		line[2][t] = neighbour.near_3 % 4;
		neighbour.near_3 = (neighbour.near_3 - (neighbour.near_3 % 4)) / 4;

		line[3][t] = neighbour.near_4 % 4;
		neighbour.near_4 = (neighbour.near_4 - (neighbour.near_4 % 4)) / 4;
	}

	for (k = 0; k <= 3; k++)
	{
		//长度为7的情形
		for (t = 0; t <= 4; t++)
		{
			//两侧为‘-’的情形
			if (playerin != 1 || (line[k][t + 0] != 1 && line[k][t + 6] != 1))
			{
				//-11111- five 9,000,000
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == playerin && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 9000000;

				//-11101- deadfour_2  3,000
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == playerin && line[k][t + 4] == 0 && line[k][t + 5] == playerin)
					basicscore += 3000;
				if (line[k][t + 1] == playerin && line[k][t + 2] == 0 && line[k][t + 3] == playerin && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 3000;

				//-11011- deadfour_3  2,600
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == 0 && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 2600;

				//-10011- deadthree_2 600
				if (line[k][t + 1] == playerin && line[k][t + 2] == 0 && line[k][t + 3] == 0 && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 600;
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == 0 && line[k][t + 4] == 0 && line[k][t + 5] == playerin)
					basicscore += 600;

				//-10101- deadthree_3 550
				if (line[k][t + 1] == playerin && line[k][t + 2] == 0 && line[k][t + 3] == playerin && line[k][t + 4] == 0 && line[k][t + 5] == playerin)
					basicscore += 550;
			}

			//边缘不对称情形
			if ((line[k][t + 0] != (3 - playerin) || line[k][t + 0] != 3) && (playerin != 1 || line[k][t + 6] != 1))
			{
				//211110- deadfour_1 2,500
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == playerin && line[k][t + 4] == playerin && line[k][t + 5] == 0)
					basicscore += 2500;

				//211100- deadthree_1 500
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == playerin && line[k][t + 4] == 0 && line[k][t + 5] == 0)
					basicscore += 500;

				//211000- deadtwo_1  150
				if (line[k][t + 1] == playerin && line[k][t + 2] == playerin && line[k][t + 3] == 0 && line[k][t + 4] == 0 && line[k][t + 5] == 0)
					basicscore += 150;
			}

			//前一种的对称
			if ((line[k][t + 6] != (3 - playerin) || line[k][t + 6] != 3) && (playerin != 1 || line[k][t + 0] != 1))
			{
				//211110- deadfour_1 2,500
				if (line[k][t + 1] == 0 && line[k][t + 2] == playerin && line[k][t + 3] == playerin && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 2500;

				//211100- deadthree_1 500
				if (line[k][t + 1] == 0 && line[k][t + 2] == 0 && line[k][t + 3] == playerin && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 500;

				//211000- deadtwo_1  150
				if (line[k][t + 1] == 0 && line[k][t + 2] == 0 && line[k][t + 3] == 0 && line[k][t + 4] == playerin && line[k][t + 5] == playerin)
					basicscore += 150;
			}
		}


		//长度为8的情形
		for (t = 0; t <= 3; t++)
		{
			//两侧为‘-01’的情形
			if ((playerin != 1 || (line[k][t + 0] != 1 && line[k][t + 7] != 1)) && line[k][t + 1] == 0 && line[k][t + 6] == 0 && line[k][t + 2] == playerin && line[k][t + 5] == playerin)
			{
				//-011110- livefour_0 3,000,000
				if (line[k][t + 3] == playerin && line[k][t + 4] == playerin)
					basicscore += 3000000;

				//-010110- livethree_2 2,000
				if (line[k][t + 3] == 0 && line[k][t + 4] == playerin)
					basicscore += 2000;
				if (line[k][t + 3] == playerin && line[k][t + 4] == 0)
					basicscore += 2000;

				//-010010- deadtwo_3  200
				if (line[k][t + 3] == 0 && line[k][t + 4] == 0)
					basicscore += 200;
			}
		}

		for (t = 0; t <= 2; t++)
		{
			//-0011100- livethree_1 3,000
			if ((playerin != 1 || (line[k][t + 0] != 1 && line[k][t + 8] != 1)) && line[k][t + 1] == 0 && line[k][t + 2] == 0 && line[k][t + 3] == playerin && line[k][t + 4] == playerin && line[k][t + 5] == playerin && line[k][t + 6] == 0 && line[k][t + 7] == 0)
				basicscore += 3000;
		}

		for (t = 0; t <= 1; t++)
		{
			//-00011000- livetwo_0  650
			if ((playerin != 1 || (line[k][t + 0] != 1 && line[k][t + 9] != 1)) && line[k][t + 1] == 0 && line[k][t + 2] == 0 && line[k][t + 3] == 0 && line[k][t + 4] == playerin && line[k][t + 5] == playerin && line[k][t + 6] == 0 && line[k][t + 7] == 0 && line[k][t + 8] == 0)
				basicscore += 650;
		}

		for (t = 0; t <= 2; t++)
		{
			//-0010100- deadtwo_2  250
			if ((playerin != 1 || (line[k][t + 0] != 1 && line[k][t + 8] != 1)) && line[k][t + 1] == 0 && line[k][t + 2] == 0 && line[k][t + 3] == playerin && line[k][t + 4] == 0 && line[k][t + 5] == playerin && line[k][t + 6] == 0 && line[k][t + 7] == 0)
				basicscore += 250;
		}
	}

	Broad[i][j] = originalstate;

	return basicscore;
}

//根据点和player，自动增加对方分数（用于堵）,如果不能下会返回-1
int AI_Score(int i, int j, int playerin)
{
	int foescore = 0;

	if (AI_Score_Distance(i, j) == -1 || AI_Score_Ban(i, j, playerin) == -1)
		return -1;
	else
	{
		if (AI_Score_Ban(i, j, (3 - playerin)) != -1)
			foescore = AI_Score_Basic(i, j, (3 - playerin));
		return (AI_Score_Distance(i, j) + 8 * AI_Score_Basic(i, j, playerin) + foescore);
	}
}




//在分数表生成i，j点分数
void AI_Score_Point(int i, int j)
{
	int score_1 = 0;
	int score_2 = 0;
	int score_distance = 0;

	score_distance = AI_Score_Distance(i, j);

	if (score_distance == -1)
	{
		ScoreTable_1[i][j] = -1;
		ScoreTable_2[i][j] = -1;
		return;
	}

	score_2 = AI_Score_Basic(i, j, 2);

	if (AI_Score_Ban(i, j, 1) == -1)
	{
		score_1 = -1;
		ScoreTable_1[i][j] = -1;
	}
	else
	{
		score_1 = AI_Score_Basic(i, j, 1);
		ScoreTable_1[i][j] = score_distance + Style_Me * score_1 + Style_Foe * score_2;
	}

	ScoreTable_2[i][j] = score_distance + Style_Me * score_2 + Style_Foe * score_1;
}

//生成分数表
void AI_Score_Table()
{
	int i, j;

	for (i = 0; i <= 14; i++)
	{
		for (j = 0; j <= 14; j++)
		{
			AI_Score_Point(i, j);
		}
	}
}

//修改落子点附近分数表
void AI_Score_Edit(int i, int j)
{
	int tempi, tempj;
	int k;

	//横向
	tempi = i;
	tempj = j;
	tempj = (tempj > 5) ? (tempj - 5) : 0;
	for (k = 0; k <= 10 && tempj <= 14; k++)
	{
		AI_Score_Point(tempi, tempj);

		tempj++;
	}

	//纵向
	tempi = i;
	tempj = j;
	tempi = (tempi > 5) ? (tempi - 5) : 0;
	for (k = 0; k <= 10 && tempi <= 14; k++)
	{
		AI_Score_Point(tempi, tempj);

		tempi++;
	}

	//左上右下
	tempi = i;
	tempj = j;
	for (k = 0; k < 5 && tempi >= 0 && tempj >= 0; k++)
	{
		tempi--;
		tempj--;
	}
	for (k = 0; k <= 10 && tempi <= 14 && tempj <= 14; k++)
	{
		AI_Score_Point(tempi, tempj);

		tempi++;
		tempj++;
	}

	//左下右上
	tempi = i;
	tempj = j;
	for (k = 0; k < 5 && tempi <= 14 && tempj >= 0; k++)
	{
		tempi++;
		tempj--;
	}
	for (k = 0; k <= 10 && tempi <= 14 && tempj <= 14; k++)
	{
		AI_Score_Point(tempi, tempj);

		tempi--;
		tempj++;
	}
}



void AI_Score_Maxfinder(int* maxi, int* maxj, int* maxvalue)
{
	int i, j;
	*maxvalue = 0;

	for (i = 0; i <= 14; i++)
		for (j = 0; j <= 14; j++)
		{
			if (player == 1)
			{
				if (ScoreTable_1[i][j] >= *maxvalue)
				{
					*maxi = i;
					*maxj = j;
					*maxvalue = ScoreTable_1[i][j];
				}
			}
			else
				if (ScoreTable_2[i][j] >= *maxvalue)
				{
					*maxi = i;
					*maxj = j;
					*maxvalue = ScoreTable_2[i][j];
				}
		}
}


int AI_Score_AlphaBeta(int depth)
{
	if (depth == SetDepth || game == 255)
		return 0;
	else
	{
		int maxvalue, i, j, val;
		int maxval = 0;

		depth++;

		AI_Score_Maxfinder(&i, &j, &maxvalue); //新函数

		if (maxvalue >= 9000000)
			if ((SetDepth - depth) % 2 == 0)
				return (AI_Score_Depth(depth) * maxvalue);

		down(i, j);
		AI_Score_Edit(i, j);

		val = AI_Score_AlphaBeta(depth);

		undo();

		AI_Score_Edit(i, j);

		if ((SetDepth - depth) % 2 == 0)
			return (val + AI_Score_Depth(depth) * maxvalue);
		else
			return val;
	}
}

void AI_Score_Down()
{
	int i, j;
	int maxi = 0;
	int maxj = 0;
	int maxvalue = 0;
	int depth = 0;
	int tempvalue = 0;

	if (game == 0)
	{
		down(7, 7);
		return;
	}

	AI_Score_Table();

	for (i = 0; i <= 14; i++)
		for (j = 0; j <= 14; j++)
		{
			AI_Score_Point(i, j);
			if ((player == 1 && ScoreTable_1[i][j] >= 0) || (player == 2 && ScoreTable_2[i][j] >= 0))
			{
				down(i, j);
				AI_Score_Edit(i, j);
	//			tempvalue = AI_Score_AlphaBeta(depth);
				tempvalue = 0;
				undo();
				AI_Score_Edit(i, j);

				if (player == 1)
					tempvalue += (AI_Score_Depth(depth) * ScoreTable_1[i][j]);
				else if (player == 2)
					tempvalue += (AI_Score_Depth(depth) * ScoreTable_2[i][j]);

				if (tempvalue > maxvalue)
				{
					maxi = i;
					maxj = j;
					maxvalue = tempvalue;
				}
			}
		}

	down(maxi, maxj);
}

//根据评分表下棋，值相等时偏向更大的


//界面，输入函数
//需要player，notecode，AI_Down函数，Broad初始化函数，down函数
void Input(int mode)
{
	int i, j;
	int k;
	char input[20];
	int tempi = 0;
	int legal = 0;

	printf("\n");
	if (game >= 1)
		printf("上一步的坐标是%c%d\n", (Record[0][2] + 'A'), (15 - Record[0][1]));

	if (player == mode)
	{
		AI_Score_Down();
		return;
	}

	printf("请输入下棋的坐标\n");
	printf("现在落子的是玩家%d\n", player);


	scanf("%s", input);

	for (k = 0; input[k] != '\0'; k++)
	{
		//开发者模式功能手动AI、跳过、直接胜利
		if (mode == 3)
		{
			if (input[k] == 'p' || input[k] == 'P')
			{
				AI_Score_Down();
				return;
			}
			else if (input[k] == 's' || input[k] == 'S')
			{
				player = 3 - player;
				return;
			}
			else if (input[k] == 'w' || input[k] == 'W')
			{
				if (input[k + 1] == '1')
				{
					notecode = 1;
					return;
				}
				else if (input[k + 1] == '2')
				{
					notecode = 2;
					return;
				}
			}
		}

		if (input[k] >= 'A' && input[k] <= 'O')
		{
			j = input[k] - 'A';
			legal++;
		}
		else if (input[k] >= 'a' && input[k] <= 'o')
		{
			j = input[k] - 'a';
			legal++;
		}
		else if (input[k] >= '0' && input[k] <= '9')
			tempi = 10 * tempi + (input[k] - '0');
		else if (input[k] == 'u' || input[k] == 'U')
		{
			undo();
			if ((mode == 1 || mode == 2) && game != 0)
				undo();
			return;
		}
		else if (input[k] == 'q' || input[k] == 'Q')
		{
			game = -1;
			return;
		}
		else if (input[k] == 'r' || input[k] == 'R')
		{
			Broad_Initialize();
			return;
		}
		else if (input[k] == 'w' || input[k] == 'W')
		{
			notecode = 9;
			return;
		}
	}

	i = 15 - tempi;

	if (legal >= 2)
	{
		notecode = 7;
		return;
	}

	if (i >= 0 && i <= 14 && j >= 0 && j <= 14)
	{
		down(i, j);
	}
	else
		notecode = 5;
}

void StartGame(int mode)
{
	Broad_Initialize();
	while (game > -1) {
		Broad_State();
		Broad_Display();
		Display_Note();
		Input(mode);
		Judge();
		system("cls");
	}
	return;
}

void Set_Default()
{
	AI_Score_Style();
	AI_Score_SetDepth();
}

void Setting_1()
{
	int k;
	char set[20];
	int tempi = 0;

	system("cls");
	printf("现在的深度为%2d，进攻参数为%2d，防守参数为%2d\n", SetDepth, Style_Me, Style_Foe);
	printf("请依次输入设置深度，进攻参数，防守参数，均为两位数\n");
	printf("输入a或A则使用自动参数（包括后面所有），输入q或Q取消设置\n");

	scanf("%s", set);
	system("cls");


	for (k = 0; set[k] != '\0'; k++)
	{
		if (set[k] >= '0' && set[k] <= '9')
			tempi = 10 * tempi + (set[k] - '0');
		else if (set[k] == 'A' || set[k] == 'a')
		{
			Set_Default();
			return;
		}
		else if (set[k] == 'Q' || set[k] == 'q')
		{
			return;
		}
	}

	Style_Foe = tempi % 100;
	tempi = (tempi - (tempi % 100)) / 100;
	Style_Me = tempi % 100;
	tempi = (tempi - (tempi % 100)) / 100;
	SetDepth = tempi;
}

void Setting_2()
{
	int k;
	char set[20];
	int tempi = 0;

	printf("现在的0~6层占比设置为%2d,%2d,%2d,%2d,%2d,%2d,%2d\n", DepthValue[0], DepthValue[1], DepthValue[2], DepthValue[3], DepthValue[4], DepthValue[5], DepthValue[6]);
	printf("请依次输入这些层分值，均为两位数\n");
	printf("输入q或Q跳过\n");

	scanf("%s", set);
	system("cls");

	for (k = 0; set[k] != '\0'; k++)
	{
		if (set[k] >= '0' && set[k] <= '9')
			tempi = 10 * tempi + (set[k] - '0');
		else if (set[k] == 'A' || set[k] == 'a')
		{
			Set_Default();

			return;
		}
		else if (set[k] == 'A' || set[k] == 'a')
		{
			return;
		}

		if (k != 0 && ((k % 2) == 1))
		{
			DepthValue[(k - 1) / 2] = tempi;
			tempi = 0;
		}
	}
}




void Interface()
{
	int k;
	char input[20];

	system("color F0");
	printf("欢迎使用假装可以下的五子棋\n\n");
	printf("0 - 人人对战\n");
	printf("1 - 人机对战（机先）\n");
	printf("2 - 人机对战（人先）\n");
	printf("3 - 开发者模式\n");
	printf("4 - 修改设置\n");
	printf("5 - 溜了溜了\n");
	printf("请选择模式或进入设置界面：\n\n");

	scanf("%s", input);

	k = input[0] - '0';
	if (input[1] != '\0')
		k = -1;

	switch (k)
	{
	case 0:
		StartGame(0);
		break;

	case 1:
		StartGame(1);
		break;

	case 2:
		StartGame(2);
		break;

	case 3:
		StartGame(3);
		break;

	case 4:
		Setting_1();
		Setting_2();
		return;
		break;

	case 5:
		exitvalue = 1;
		break;
	}

	printf("\n请重新输入\n");
}



main()
{
	Set_Default();

	while (exitvalue == 0)
		Interface();
}
