#include "head.h"
#include  <stdlib.h>
																	//���Ժ���
struct point AI_White_0(int n){
	int candidates_white(long long int []);//����ѡ����ѡ�㣬�浽MAX����
	struct point zl;
	long long int pmax[8];//���ڼ�¼��ߵļ�������
																	//�ҵ��ķ���ߵ�λ��
	int l;	
	int s=0,w;

	l=candidates_white(pmax);	

	struct point val;//������¼ÿһ�����Ӻ󣬶��ַ��ص����ӵ�λ�ú���ߵķ�
	int x,y;//������ʱ��¼������������ӵ�λ��
	long long int tempmax=-34867844010*4;//������¼��̽һ�������ߵķ֣���ʼ��һ����Сֵ
	if(n>0)
	{
		for(int i=0;i<l;i++)													//��ÿһ���÷ֽϸߵ��Ӷ���һ��
		{
			x=WhiteGradesMax[i].x;
			y=WhiteGradesMax[i].y;
			aRecordBoard[x][y]=2;//����һ������

			Info(aRecordBoard,x,y);

			if(winner_2(x,y))
			{
				aRecordBoard[x][y]=0;//��ԭ����
				zl.x=x;
				zl.y=y;
				zl.score=3000000000;
				return zl;
			}
	
			val=AI_Black_0(n-1);
		
			if(val.score==tempmax)//����ҵ��͵�ǰ��߷���һ���ĵ�ͱ�������
			{
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}

			if(val.score>tempmax)//����ҵ��ȵ�ǰ��ߵķָߵķ������Ͱ�֮ǰ�����;
			{
				s=0;
				tempmax=val.score;
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}
			aRecordBoard[x][y]=0;//��ԭ����
			
		}
		w=(rand()%s);					

		zl.x=bestpwhite[w].x;
		zl.y=bestpwhite[w].y;
		zl.score=tempmax;	

		return zl;
	
	}	


/*
	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) < dmin)		//�����ĵľ���	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) == dmin)
			{
				bestpwhite[l].x=WhiteGradesMax[l].x;
				bestpwhite[l].y=WhiteGradesMax[l].y;
			}
*/
			
	int z;
	z=(rand()%l);					//����ж�����ŵĵ�������һ��

	zl.x=WhiteGradesMax[z].x;
	zl.y=WhiteGradesMax[z].y;
	zl.score=pmax[0];	

	return zl;
		
}


																		//���԰���
struct point AI_White_1(int n){
	int candidates_white(long long int []);//����ѡ����ѡ�㣬�浽MAX����
	struct point zl;
	long long int pmax[8];//���ڼ�¼��ߵļ�������
																	//�ҵ��ķ���ߵ�λ��
	int l;	
	int s=0,w;

	l=candidates_white(pmax);	

	struct point val;//������¼ÿһ�����Ӻ󣬶��ַ��ص����ӵ�λ�ú���ߵķ�
	int x,y;//������ʱ��¼������������ӵ�λ��
	long long int tempmax=-34867844010*4;//������¼��̽һ�������ߵķ֣���ʼ��һ����Сֵ
	if(n>0)
	{
		for(int i=0;i<l;i++)													//��ÿһ���÷ֽϸߵ��Ӷ���һ��
		{
			x=WhiteGradesMax[i].x;
			y=WhiteGradesMax[i].y;
			aRecordBoard[x][y]=2;//����һ������

			Info(aRecordBoard,x,y);

			if(winner_2(x,y))
			{
				aRecordBoard[x][y]=0;//��ԭ����
				zl.x=x;
				zl.y=y;
				zl.score=34867844010;
				return zl;
			}

			val=AI_Black_1(n-1);
			
			if(val.score==tempmax)//����ҵ��͵�ǰ��߷���һ���ĵ�ͱ�������
			{
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}

			if(val.score>tempmax)//����ҵ��ȵ�ǰ��ߵķֵ͵ķ������Ͱ�֮ǰ�����;
			{
				s=0;
				tempmax=val.score;
				bestpwhite[s].x=x;
				bestpwhite[s].y=y;
				s++;	
			}
			aRecordBoard[x][y]=0;//��ԭ����
			
		}
		w=(rand()%s);					

		zl.x=bestpwhite[w].x;
		zl.y=bestpwhite[w].y;
		zl.score=tempmax;	

		return zl;
	
	}	


/*
	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) < dmin)		//�����ĵľ���	
			dmin=distance;

	for(int l=0;l<250;l++)
		if((distance = (WhiteGradesMax[l].x-8)*(WhiteGradesMax[l].x-8)+(WhiteGradesMax[l].y-8)*(WhiteGradesMax[l].y-8)) == dmin)
			{
				bestpwhite[l].x=WhiteGradesMax[l].x;
				bestpwhite[l].y=WhiteGradesMax[l].y;
			}
*/
			
	int z;
	z=(rand()%l);					//����ж�����ŵĵ�������һ��

	zl.x=WhiteGradesMax[z].x;
	zl.y=WhiteGradesMax[z].y;
	zl.score=pmax[0];	

	return zl;
		
}


int candidates_white(long long int pmax[8]){

	int count=0;//ѡ��8����ѡ�㣬��ʼΪ0

	for(int i=0;i<8;i++)
		pmax[i]=0;

	for(int i=0;i<250;i++)
		{
			WhiteGradesMax[i].x=0;
			WhiteGradesMax[i].y=0;	
		}

	for(int i=0;i<250;i++)
		{
			bestpwhite[i].x=0;
			bestpwhite[i].y=0;	
		}


	for(int i=1;i<16;i++)
		for(int j=1;j<16;j++)
			gradestable_white[i][j].score=get_grades_white(i,j);
																//�ҳ���ߵķ�
	int g;
	while(count<4){
		for(int i=1;i<16;i++)
			for(int j=1;j<16;j++)
				if(pmax[0]<gradestable_white[i][j].score)
					pmax[0]=gradestable_white[i][j].score;

		for(int i=1;i<16;i++)
			for(int j=1;j<16;j++)
				if(pmax[0]==gradestable_white[i][j].score)
				{
					WhiteGradesMax[count].x=i;
					WhiteGradesMax[count].y=j;
					count++;
				}
		if(count>3)
			break;
	
		for(int k=1;count<4;k++)
		{
			for(int i=1;i<16;i++)
				for(int j=1;j<16;j++)
					if(pmax[k]<(g=gradestable_white[i][j].score) && g<pmax[k-1])
						pmax[k]=gradestable_white[i][j].score;
	
			for(int i=1;i<16;i++)
				for(int j=1;j<16;j++)
					if(pmax[k]==gradestable_white[i][j].score)
					{
						WhiteGradesMax[count].x=i;
						WhiteGradesMax[count].y=j;
						count++;
					}		
		}
	}

	return count;		
}










