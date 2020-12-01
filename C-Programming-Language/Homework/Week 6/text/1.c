#include <stdio.h>
int main()
{	int sxhn(int n);
	int i;
	for(i=100;i<1000;++i)
	{if(sxhn(i)==1)
		printf("%d\n",i);
				}
	return 0;
}
int sxhn(int n)
{	int s[3];
	s[0]=n%10;
	s[1]=(n/10)%10;
	s[2]=((n/10)/10)%10;
	if((s[0]*s[0]*s[0]+s[1]*s[1]*s[1]+s[2]*s[2]*s[2])==n)
		return 1;
	else
		return 0;
}
