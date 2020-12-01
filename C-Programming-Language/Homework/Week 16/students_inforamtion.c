#include <stdio.h>
#define MAXNAME 25
#define MAXID 20
#define MAXNUM 10

struct info {
	char id[MAXID];
	char name[MAXNAME];
	int gender;
	int age;
}s[10];

void qsort(struct info [],int ,int);

int main(){
	for(int i=0;i<MAXNUM;i++)
	{
		system("clear");
		printf("student %d:\n",i+1);
		printf("input ID: ");scanf("%s",s[i].id);
		printf("input name: ");scanf("%s",s[i].name);
		printf("input gender (1 for male & 2 for female): ");scanf("%d",&s[i].gender);
		printf("input age: ");scanf("%d",&s[i].age);
	}

	qsort(s,0,MAXNUM-1);
	system("clear");
	printf("%-25s%-20s%-10s%-5s\n","Name","ID","Gender","age");

	for(int i=0;i<MAXNUM;i++)
	{
		printf("%-25s%-20s%-10s%-3d\n",s[i].name,s[i].id,(s[i].gender==1)?"male":"female",s[i].age);
	}
	return 0;
}

void qsort(struct info v[],int left,int right){
	int i,last;
	void swap(struct info v[],int i,int j);
	
	if(left>=right)
		return;
	swap(v,left,(left+right)/2);
	last=left;
	for(i=left+1;i<=right;i++)
		if(v[i].age<v[left].age)
			swap(v,++last,i);
	swap(v,left,last);
	qsort(v,left,last-1);
	qsort(v,last+1,right);
}

void swap(struct info v[],int i,int j){
	struct info temp;
	temp=v[i];
	v[i]=v[j];
	v[j]=temp;	
}
