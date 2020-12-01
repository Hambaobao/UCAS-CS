#include <stdio.h>

struct point{
	int x;
	int y;
} p1;

struct {
	char name[10];
	int gender;
	int age;
} laowang={"laowang",1,28};

int main(){
	struct point p2={5,10},p3={2,7},p4;
	printf("size of struct point=%ld\n",sizeof(struct point));
	printf("size of laowang=%ld\n",sizeof(laowang));
	p1.x=(p2.x+p3.x)/2;
	p1.y=(p2.y+p3.y)/2;
	p4=p1;
	printf("middle point of p1=(%d,%d)\n",p4.x,p4.y);
	printf("information of laowang:\n");
	printf("name:%s gender:%s age:%d\n",laowang.name,laowang.gender==1?"male":"female",laowang.age);
	return 0;
}
