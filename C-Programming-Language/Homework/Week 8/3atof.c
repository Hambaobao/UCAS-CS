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

double atof(char s[]){
	double num,power;
	int i,sign;
	for(i=0;isspace(s[i]);i++)
		;
	sign=(s[i]=='-')?-1:1;
	if(s[i]=='-'||s[i]=='+')
		i++;
	for(num=0.0;isdigit(s[i]);i++)
		num=10.0*num+s[i]-'0';
	if(s[i]=='.')
		i++;
	for(power=1.0;isdigit(s[i]);i++)
		{
			num=10.0*num+s[i]-'0';
			power*=10.0;
					}
	return sign*(num/power);
}

int mygetline(char s[],int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i;
}
