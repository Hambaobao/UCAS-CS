#include <stdio.h>

#define LEN 3

int main(){
	void swap(int [],int,int); 
	int s[LEN],m;
	scanf("%d%d%d",&s[0],&s[1],&s[2]);
	for(int j=LEN-1;j>=0;j--){
		for(int i=0;i<j;i++)
			if(s[i]<s[i+1])
				swap(s,i,i+1);
		}	
	printf("%d %d %d\n",s[0],s[1],s[2]);
	return 0;
}

void swap(int s[3],int i,int j){
	int m;
	m=s[i];
	s[i]=s[j];
	s[j]=m;
}
