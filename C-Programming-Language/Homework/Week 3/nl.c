#include <stdio.h>
int main()
{	double nl;
	for(nl=0;getchar()!=EOF;)
		if (getchar()=='\n')
			++nl;
	printf("\nthe total number:%.0f\n",nl);
	return 0;
}
