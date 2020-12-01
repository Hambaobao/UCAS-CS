#include <stdio.h>

static char daytab[2][13]={
	{0,31,28,31,30,31,30,31,30,31,30,31,30},
	{0,31,29,31,30,31,30,31,30,31,30,31,30},

};

int is_leap_year(int year);
int day_of_year(int year,int month,int day);
void month_day(int year,int yearday,int *pmonth,int *pday);


int main(){
	int mode;
	int year,month,day;
	int yearday;

	printf("please choose:\n");
	printf("(1) year-month-day to yearday: press 1\n");
	printf("(2) yearday to year-month-day: press 2\n");
	scanf("%d",&mode);
	if(mode==1)
	{
		printf("input year:");scanf("%d",&year);
		printf("input month:");scanf("%d",&month);
		printf("input day:");scanf("%d",&day);
		yearday=day_of_year(year,month,day);
		printf("yearday = %d\n",yearday);
	}
	else if(mode==2)
	{
		printf("input year:");scanf("%d",&year);
		printf("input yearday:");scanf("%d",&yearday);
		month_day(year,yearday,&month,&day);
		printf("year=%d month=%d day=%d\n",year,month,day);
	}
	else
	{
		printf("Wrong mode!\n");	
	}
	return 0;
}

int is_leap_year(int year){
	int leap;
	leap=((year%4 == 0 && year%100!= 0) || (year%400 == 0)?1:0);
	return leap;
}

int day_of_year(int year,int month,int day){
	int i,leap;
	leap=is_leap_year(year);
	for(i=1;i<month;i++)
		day+=daytab[leap][i];
	return day;
}

void month_day(int year,int yearday,int *pmonth,int *pday){
	int i,leap;
	leap=is_leap_year(year);
	for(i=1;yearday>daytab[leap][i];i++)
		yearday-=daytab[leap][i];
	*pmonth=i;
	*pday=yearday;
}



















