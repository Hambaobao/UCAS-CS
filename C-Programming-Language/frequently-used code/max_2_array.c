#include <stdio.h>

#define MAXLEN 1000

int mygetline(char input[]);

int count(char input[]);

void copy(char from[],char to[]);

int main()
{	int a=0;
	int b=0;   
	char shortest[MAXLEN];
	char se_shortest[MAXLEN];
	char input[MAXLEN];
	int d=mygetline(input);
	a=d;
	copy(input,shortest);
	d=mygetline(input);
	if (d<a)
	{	copy(shortest,se_shortest);
		copy(input,shortest);
		b=a;
		a=d;
			}
	else {
		copy(input,se_shortest);
		b=d;	}
	
	while ((d=mygetline(input))>0)         
	{	
		if (d<a)		
		{	b=a;
			a=d; 
			copy(shortest,se_shortest);
			copy(input,shortest);
				}
		else if (d<b)       
		{	
			b=d;
			copy(input,se_shortest);
				}
		else
			;
	}
		 printf("\n");
		 printf("%s",shortest);
		 printf("%s",se_shortest);
	return 0;	
}
int mygetline(char input[])
{	int i;
	char c;
	for(i=0;i<=MAXLEN-1&&(c=getchar())!=EOF&&c!='\n';++i)
		input[i]=c;
	if(c=='\n')
	{	input[i]=c;
		++i;
	}
	input[i]='\0';
	return i;
}



int count(char input[])
{	int i=0;
	while (input[i]!='\0')
	{	
		i++;
	}
	return i;
}

void copy(char from[],char to[])
{	int i=0;
	while((to[i]=from[i])!='\0')
		i++;
	}
