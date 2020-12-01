#include <stdio.h>
int main()
{	int c;	
	while((c=getchar())!=EOF)
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
		if(c>='a'&&c<='z')
		putchar(c-32);
		else
		putchar(c+32);
		}else putchar(c);
	return 0;
}
