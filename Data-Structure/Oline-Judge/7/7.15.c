#include <stdio.h>
#define MAXSIZE 10
#define MAXLINE 20

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

int main(){
	int A[MAXSIZE][MAXSIZE];
	int effc[MAXSIZE];
	int OUT[MAXSIZE*MAXSIZE];
	char opration[MAXLINE];
	int nodenumb;
	int v,w;
	char c;

	for(int i=0;i<MAXSIZE;i++)
		for(int j=0;j<MAXSIZE;j++){
			A[i][j]=0;
		}

	for(int i=0;i<MAXSIZE*MAXSIZE;i++)
		OUT[i]=0;

	for(int i=0;i<MAXSIZE;i++)
		effc[i]=0;


	scanf("%d",&nodenumb);
	getchar();

	do{
		scanf("%d",&v);
		getchar();
		scanf("%d",&w);
		A[v][w]=1;
		effc[v]=1;
		effc[w]=1;
		c=getchar();
	}while(c!='\n');

	while((c=getchar())!=EOF){
		mygetline(opration,MAXLINE);

		if(c=='I')
		{
			if(opration[0]=='V')
			{
				v=opration[2]-'0';
				effc[v]=1;
				nodenumb++;
				continue;
			}

			if(opration[0]=='A')
			{
				v=opration[2]-'0';
				w=opration[4]-'0';
				A[v][w]=1;
				continue;
			}
		}

		if(c=='D')
		{
			if(opration[0]=='V')
			{
				v=opration[2]-'0';
				effc[v]=0;
				nodenumb--;
				continue;
			}

			if(opration[0]=='A')
			{
				v=opration[2]-'0';
				w=opration[4]-'0';
				A[v][w]=0;
				continue;
			}
		}
	}

	int k=0;
	for(int i=0;i<MAXSIZE;i++)
	{
		if(effc[i])
		{
			for(int j=0;j<MAXSIZE;j++)
				if(effc[j])
					OUT[k++]=A[i][j];
		}
	}


	k=0;
	for(int i=0;i<nodenumb-1;i++){
		for(int j=0;j<nodenumb-1;j++)
			printf("%d,",OUT[k++]);
		printf("%d\n",OUT[k++]);
	}

	for(int i=0;i<nodenumb-1;i++)
		printf("%d,",OUT[k++]);
	printf("%d",OUT[k]);

	return 0;
}