#include <stdio.h>
#define WIDTH 200
char add1[WIDTH];
char add2[WIDTH];
char sum[WIDTH];

int mygetline(char [],int );
int max(int ,int );
int add(char [],char [],char [],int ,int ,int );

int main(){
	int a;
	int b;
	int m,n;
	printf("add1 = : ");
	a=mygetline(add1,WIDTH)-1;
	printf("add2 = : ");
	b=mygetline(add2,WIDTH)-1;
	m=max(a,b);
	n=add(add1,add2,sum,a,b,m);
	printf("sum = : ");
	for(int i=n;i<WIDTH-1;i++)
		printf("%c",sum[i]);
	printf("\n");
	return 0;
}

int mygetline(char s[],int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i;
}

int max(int a,int b){
	if(a>=b)
		return a;
	if(b>a)
		return b;
}

int add(char add1[],char add2[],char sum[],int a,int b,int m){
	int plus1,plus0,plus2;
	int cout0=0,cout1=0;
	int n,i;
	for(int i=0;i<WIDTH;i++)
		sum[i]='0';
	for(i=0 ;!(a<0 && b<0);i++,a--,b--)
	{
		plus0=((add1[a+1]-'0')+(add2[b+1]-'0')+cout1);

		if(i==0)
			cout0=0;
		else if(i>0)
			cout0=(plus0>9)?1:0;

		plus1=((add1[a]-'0')+(add2[b]-'0')+cout0);
		cout1=(plus1>=10)?1:0;
		
		plus2=(plus1%10);
		
		if(a>=0 && b>=0)
		{
			sum[WIDTH-1-i]=plus2+'0';
		}
		else if(a<0 && b>=0)
		{
			sum[WIDTH-1-i]=add2[b];
		}
		else if(a>=0 && b<0)
		{
			sum[WIDTH-1-i]=add1[a];
		}	
	}
	sum[WIDTH-1-i]=cout1+'0';
	return (cout1==0)?WIDTH-1-i+1:WIDTH-1-i;	
}





























