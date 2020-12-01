#include <stdio.h>

#define DATANUM 15

int binsearch(int x, int v[], int n);

int main(){
	int data[]={1,2,4,10,29,33,35,48,54,56,61,62,63,70,79};
	int x,index;
	scanf("%d",&x);
	index=binsearch(x,data,DATANUM);
	if (index!=-1)
		{
		printf("index=%d\n",index);
							}
	else {
			printf("%d is not in the array\n",x);
							}
	return 0;
}

int binsearch(int x, int v[], int n){
	int low,high,mid;
	low=0;
	high=n-1;
		while(low<=high){
			mid=(high+low)/2;
			if (x==v[mid])
				{
				return mid;
							}
			else if (x<v[mid])
					{
					high=mid-1;
								}
			else 
					{
					low=mid+1;
								}
			}
	return -1;
}
