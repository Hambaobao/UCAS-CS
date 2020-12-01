#include <stdio.h>

int atoi(char s[]);
int main(){
	char s1[]="416";
	char s2[]="   416";
	char s3[]="   -416";
	char s4[]="  + 416";
	char s5[]="-4 1 6";
	int d;
	d=atoi(s1);
	printf("s1=\"%s\", atoivalue=%d\n",s1,d);
	d=atoi(s2);
	printf("s2=\"%s\", atoivalue=%d\n",s2,d);
	d=atoi(s3);
	printf("s3=\"%s\", atoivalue=%d\n",s3,d);
	d=atoi(s4);
	printf("s4=\"%s\", atoivalue=%d\n",s4,d);
	d=atoi(s5);
	printf("s5=\"%s\", atoivalue=%d\n",s5,d);
	return 0;
}
