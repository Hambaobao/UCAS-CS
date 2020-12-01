#include <stdio.h>
#define MAXLINE	 100

char pattern []="ould";

int mygetline(char [],int );

int strindex(char [],char []);

int main(){
	char line[MAXLINE];
	int found=0;
	while (mygetline(line,MAXLINE)>=0)
		if (strindex(line,pattern)>=0){
			printf("%s",line);
			found++;
		}
	return found;	
}
