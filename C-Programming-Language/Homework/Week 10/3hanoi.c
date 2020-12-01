#include <stdio.h>
int main(){
	void hanoi(int n,char a,char b,char c);
	int m;
	printf("input the number of disks:");
	scanf("%d",&m);
	printf("the step to move %d disks:\n",m);
	hanoi(m,'A','B','C');
	return 0;
}

void hanoi(int n,char a,char b,char c){
	void move(char x,char y);
	if(n==1)
		move(a,c);
	else{
		hanoi(n-1,a,c,b);
		move(a,c);
		hanoi(n-1,b,a,c);
	}
}

void move(char x,char y){
	printf("%c->%c\n",x,y);
}
