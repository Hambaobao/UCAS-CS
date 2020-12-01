#include <stdio.h>

int main(){
	char amessage[]="now is the time";
	char *pmessage="now is the time";
	printf("amessage=%s\n",amessage);
	amessage[0]='t';
	printf("amessage=%s\n",amessage);
	printf("pmessage=%s\n",pmessage);
	pmessage[0]='t';
	printf("pmessage=%s\n",pmessage);
	return 0;
}
