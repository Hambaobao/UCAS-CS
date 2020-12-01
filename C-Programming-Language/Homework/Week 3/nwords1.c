#include <stdio.h>
int main()
{	int nw,state,c;
	nw=0;
	state=0;
	c=getchar();
	while(c!=EOF){
		if (c==' '||c=='\t'||c=='\n'){
		state=0;
		}else {
			if (state==0){
			++nw;
			state=1;
			}
		}
		c=getchar();
	}
	printf("\nthe total number of words:%d\n",nw);
	return 0;

}
