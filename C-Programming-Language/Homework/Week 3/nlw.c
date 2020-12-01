#include <stdio.h>
int main()
{	int nw,state,c,nl;
	nw=nl=0;
	state=0;
	while((c=getchar())!=EOF){
		if (c=='\n'){
			nl++;}
		if (c==' '||c=='\t'||c=='\n'){
		state=0;
		}else {
			if (state==0){
			++nw;
			state=1;}
		}
	}
	printf("\nthe taotal number of lines:%d the total number of words:%d\n",nl,nw);

}
