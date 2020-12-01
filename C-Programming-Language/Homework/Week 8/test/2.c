#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000

int mygetline(char [],int );

double atof(char []);

int i;

char input[MAXLINE];

int main(){
	double d;
	int pnum,sign1;
	mygetline(input,MAXLINE);
	d=atof(input);
	if(input[i]=='e')
		{
			i++;
		sign1=(input[i++]=='-')?-1:1;
			for(pnum=0;isdigit(input[i]);i++)
				pnum=10*pnum+input[i]-'0';
			if(sign1=-1)				
				printf("input = %lfe-%d\n",d,pnum);
			else
				printf("input = %lfe%d\n",d,pnum);
								}
	else  if(input[i]=='E')
		{
			i++;
		sign1=(input[i++]=='-')?-1:1;
			for(pnum=0;isdigit(input[i]);i++)
				pnum=10*pnum+input[i]-'0';
			if(sign1=-1)				
				printf("input = %lfE-%d\n",d,pnum);
			else
				printf("input = %lfE%d\n",d,pnum);
								}
	
	return 0;
}

double atof(char s[]){
	double num,power;
	int sign;
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
{	int p,c;
	p=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[p++]=c;
	if(c=='\n')
		s[p++]='\n';
	s[p]='\0';
	return p;
}
