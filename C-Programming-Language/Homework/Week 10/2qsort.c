#include <stdio.h>
#define LEN 20

void qsort(int v[],int left,int right);

int main(){
	int data [LEN]={32,3,4,7,23,5,243,867,24,654,3245,125,56,325,74,5645,621,25,26,6};
	int i;
	qsort(data,0,LEN-1);
	for (i=0;i<LEN;i++)
		printf("%d ",data[i]);
	printf("\n");
	return 0;	
}

void qsort(int v[],int left,int right){
	int i,last;
	void swap(int v[],int i,int j);
	
	if(left>=right)
		return;
	swap(v,left,(left+right)/2);
	last=left;
	for(i=left+1;i<=right;i++)
		if(v[i]<v[left])
			swap(v,++last,i);
	swap(v,left,last);
	qsort(v,left,last-1);
	qsort(v,last+1,right);
}

void swap(int v[],int i,int j){
	int temp;
	temp=v[i];
	v[i]=v[j];
	v[j]=temp;	
}
