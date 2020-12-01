#include <stdio.h>
int main()
{	int nw,state,c;
	nw=0;
	state=0;
	while((c=getchar())!=EOF){
		if (c==' '||c=='\t'||c=='\n'){
		state=0;
		}else {
			if (state==0){
			++nw;
			state=1;}
		}
	}
	printf("\nthe total number of words:%d\n",nw);

}
