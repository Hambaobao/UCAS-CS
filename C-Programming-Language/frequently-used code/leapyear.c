#include <stdio.h>
int main(){
	int isLeapYear(int year);
	int year,lower,upper;
	lower=2000;
	upper=2100;
	year=lower;
	while(year<=upper){
	if(isLeapYear(year)==1)
		printf("%d is leapyear\n",year);
	else
		printf("%d is not leapyear\n",year);
	year=year+1;}
	return 0;
}
/*int isLeapYear(int year){
	int r;
	r=year%4;
	if(r!=0){
		return 0;}
	else{
		r=year%100;
		if(r!=0){
			return 1;}
		else{
			r=year%400;
			if(r==0){
				return 1;}
			else{
				return 0;
			}
		}
	}
}			*/
	
int is_leap_year(int year){
	int leap;
	leap=((year%4 == 0 && year%100!= 0) || (year%400 == 0)?1:0);
	return leap;
}
