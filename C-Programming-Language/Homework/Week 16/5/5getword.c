#include <stdio.h>
#include <ctype.h>
int getword(char *word,int lim)
{
	int c,getch(void);
	void ungetch(int);
	char *w=word;
	while(isspace(c=getch()))
		;
	if(c!=EOF)
		*w++=c;
	if(!isalpha(c)){
		*w='\0';
		return c;
	}
	for( ; --lim; w++)
	{
		if(!isalnum(*w=getch())){
			ungetch(*w);
			break;
		}
	}
	*w='\0';
	return word[0];
}
