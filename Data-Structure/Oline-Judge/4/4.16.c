#include <stdio.h>

#define MAXLEN 100

int main(){
	char s[MAXLEN];
	char t[MAXLEN];
	char c;
	int lens=0,lent=0;
	int i=0;
	
	do{
		do{
			scanf("%c",&s[lens++]);
		}while(s[lens-1]!=' ');
		lens--;
		
		do{
			scanf("%c",&t[lent++]);
		}while(t[lent-1]!='\n');
		lent--;

			while(s[i]==t[i]&&i<=lens&&i<=lent)
				i++;
			if(lens==lent&&i==lens)
				printf("%d",0);
			else if(s[i]>t[i])
				printf("%d",1);
			else if(s[i]<t[i])
				printf("%d",-1);
			else if(lens>lent)
				printf("%d",1);
			else if(lens<lent)
				printf("%d",-1);			

	}while(c=getchar()!=EOF);
	return 0;
}
