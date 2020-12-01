#include <stdio.h>
#define MAXLEN 100
int main(){
	int n;
	int day[MAXLEN];
	double left[MAXLEN];
	int percent[MAXLEN];
	double d_percent[MAXLEN];
	int cups[MAXLEN];
	double d_cups[MAXLEN];
	scanf("%d",&n);
	for(int i=1;i<=n+1;i++)
		day[i]=i;
	for(int i=1;i<=n;i++)
		cups[i]=i;
	for(int i=1;i<=n;i++)
		percent[i]=i;
	for(int i=0;i<MAXLEN;i++)
		{
			left[i]=0;
			d_percent[i]=0;
			d_cups[i]=0;
					}
	left[n+1]=0.25;
	for(int i=n;i>0;i--)
		{
			left[i]=(left[i+1]+i*0.25)*(i+4)/(i+3);
								}
	for(int i=n;i>1;i--)
		{
			d_percent[i]=0.25*(1.0/(i+4));
								}
	for(int i=n;i>0;i--)							
		{	
			d_cups[i]=0.25*i;
						}
	printf("input n = %d\n",n);
	printf("total=%lf\n",left[1]);
	printf("day\tleft\t\t1/(n+4)\t1/(n+4)*0.25\tCUPs\tcup*0.25\n");
	for(int i=1;i<=n;i++)
		{
			printf("day%d\t",day[i]);
			printf("%lf\t",left[i]);
			printf("1/%d\t",percent[i]+4);
			printf("%lf\t",d_percent[i]);
			printf("CUPs%d\t",cups[i]);
			printf("%lf\n",d_cups[i]);
							}
	printf("day%d\t",day[n+1]);
	printf("%lf\n",left[n+1]);
	return 0;
}
























