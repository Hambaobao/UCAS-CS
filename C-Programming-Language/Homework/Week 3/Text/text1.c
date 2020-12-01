#include <stdio.h>
void insertsort(int s[],int len);
int main(){
	int MAXLEN;
	scanf("%d",&MAXLEN);//将键盘的输入赋值给MAXLEN
	int s1[1000]={0,};
 	int s2[1000]={0,};
	int s3[1000]={0,};
	for (int i=0;i<MAXLEN;i++)
		{
			scanf("%d",&s1[i]);
		}
	for (int i=0;i<MAXLEN;i++)
		{
			scanf("%d",&s2[i]);
		}
	for (int i=0;i<MAXLEN;i++)
		{
			scanf("%d",&s3[i]);
		}
	insertsort(s1,MAXLEN);
	for (int i=0;i<MAXLEN;i++)
		{
		printf("%d,",s1[i]);
		}
	printf("\n");

	insertsort(s2,MAXLEN);
	for (int i=0;i<MAXLEN;i++)
		{
		printf("%d,",s2[i]);
		}
	printf("\n");

	insertsort(s3,MAXLEN);
	for (int i=0;i<MAXLEN;i++)
		{
		printf("%d,",s3[i]);
		}
	printf("\n");
return 0;
}
void insertsort(int s[],int len)
{	int i,j;
	int x;;	
	for (i=1;i<len;i++){
		for (j=i-1;j>=0;j--)
			{
		if (j>=0&&s[j+1]<s[j])
			{
			x=s[j+1];
			s[j+1]=s[j];
			s[j]=x;
			}
				
		}
	}
}
