#include <stdio.h>
int main()
{	int c;	
	while((c=getchar())!=EOF)
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
		if(c>='a'&&c<='z')
		{if (c>='a'&&c<='u')
			putchar(c+5);
		else
			putchar(c-21);}
		else
		{if (c>='A'&&c<='U')
			putchar(c+5);
		else
			putchar(c-21);
			}
		}else putchar(c);
	return 0;
}

