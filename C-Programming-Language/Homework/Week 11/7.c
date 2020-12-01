#include <stdio.h>
#include <ctype.h>
#define SIZE 10
#define BUFSIZE 100

int array[SIZE];
int bufp=0;
char buf[BUFSIZE];
 
int main(){
	int n;
	int getint(int*);
	for(n=0;n<SIZE && getint(&array[n])!=EOF;n++)
		;
	for(n=0;n<SIZE;n++)
		printf("array[%d]=%d\n",n,array[n]);
	return 0;
}

int getint(int *pn){
	int getch(void);
	void ungetch(int);
	int c,sign;
	while(isspace(c=getch()))
		;
	if(!isdigit(c) && c!=EOF && c!='+'&& c!='-'){
		ungetch(c);
		return 0;
	}
	sign=(c=='-')?-1:1;
	if(c=='+' || c=='-')
		c=getch();
	for(*pn=0;isdigit(c);c=getch())
		*pn=10* *pn+(c-'0');
	*pn*=sign;
	if(c!=EOF)
		ungetch(c);
	return c;
}


int getch(void){ 
	return (bufp>0)?buf[--bufp]:getchar();
}

void ungetch(int c){
	if(bufp>=BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++]=c;
}
