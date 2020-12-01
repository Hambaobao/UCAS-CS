#include<stdio.h>

#define MAX_SIZE 1000

int count(char input[]);
void copy(char from[],char to[]);
int mygetline(char input[]);

int main()
{
	int shortest = 0;
	int second = 0;
	 char shortest_string[MAX_SIZE];
	 char second_string[MAX_SIZE];
         char input[MAX_SIZE];
	int h=mygetline(input);
	shortest =  h;
	copy(input,shortest_string);
	h=mygetline(input);
	if(h<shortest)
	{
		copy(shortest_string,second_string);
		copy(input,shortest_string);
		second=shortest;
		shortest=count(input);
	}
	else
	{
		copy(input,second_string);
		second=h;
	}

	//for(int i=0;i<5;i++)
	while((h=mygetline(input))>0)
	{
		if(h<shortest)
		{
			second=shortest;
			shortest=h;
			copy(shortest_string,second_string);
			copy(input,shortest_string);
		}
		else if(h<second)
		{
			second=h;
			copy(input,second_string);
		}
		else
			;
	}
	printf("\n");
	printf("%s",shortest_string);
	printf("%s",second_string);
	return 0;
}

int mygetline(char input[])
{
    
	int i;
	char c;
	for(i=0;i<MAX_SIZE-1&&(c=getchar())!=EOF&&c!='\n';++i)
		input[i]=c;
	if(c=='\n')
	{
		input[i]=c;
		++i;
	}
	input[i]='\0';
	return i;
}
    
    	
    

int count(char input[])
{
	int count=0;
	while(input[count]!='\0')
	{
		count++;
	}
	return count;
}

void copy(char from[],char to[])
{
	int i=0;
	while((to[i]=from[i])!='\0')
	i++;
}

