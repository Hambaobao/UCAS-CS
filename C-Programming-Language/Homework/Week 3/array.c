#include <stdio.h>
int main(){
	int array[10];
	int ns,ot,c,i;
	ns=ot=0;
	for (i=0;i<=9;i++)
		array[i]=0;
	while((c=getchar())!=EOF)
		{
		if (c>='0'&&c<='9')
		{
		array[c-'0']++;
				}
		else {
			if (c==' '||c=='\t'||c=='\n')
			ns++;
			else ot++;
				}
		}
	for (i=0;i<=9;i++)
	printf("the total number of %d is %d\n",i,array[i]);
	printf("the total number of white is %d\n",ns);
	printf("the total number of other is %d\n",ot);
	return 0;
}
