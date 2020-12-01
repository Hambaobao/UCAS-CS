#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000

int mygetline(char [],int );

double atof(char []);

int main(){
	char input[MAXLINE];
	double d;
	mygetline(input,MAXLINE);
	d=atof(input);
	printf("%s",input);
	printf("%lf\n",d);
	return 0;
}
