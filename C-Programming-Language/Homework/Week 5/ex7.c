#include<stdio.h>

int lower(int);

int main(){
	int uc,lc;
	uc=getchar();
	lc=lower(uc);
	printf("lc=%c\n",lc);
	return 0;
}
int lower(int c){
	if(c>='A'&&c<='Z')
		return c-'A'+'a';
	else
		return c;
}
