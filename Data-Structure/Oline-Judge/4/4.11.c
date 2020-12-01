#include <stdio.h>

#define MAXLEN 100

int main(){
	char s[MAXLEN];
	char t[MAXLEN];
	char new[MAXLEN];
	int num[MAXLEN],i=0;
	int lens=0,lent=0;
	char c;
	
	do{
		do{
			scanf("%c",&s[lens++]);
		}while(s[lens-1]!=' ');
		lens--;
	
		do{
			scanf("%c",&t[lent++]);
		}while(t[lent-1]!='\n');
		lent--;
	
		int m=0;
		for(int i=0;i<lent;i++){
			for(int j=0;j<lens;j++)
				if(s[j]==t[i])
					s[j]='$';
		}
	
		for(int i=0;i<lens;i++){
			if(s[i]!='$'){
				new[m]=s[i];
				num[m]=i+1;		
			for(int j=0;j<lens;j++){
				if(s[j]==new[m])
					s[j]='$';
				}
			m++;
			}
			else
				;
		}
	
		if(m>0){
			for(int i=0;i<m;i++)
				printf("%c",new[i]);
		
			printf("%c",' ');
	
			for(int i=0;i<m;i++)
				printf("%d",num[i]-1);
		}else
			printf("%d",-1);

	}while(c=getchar()!=EOF);

	return 0;
}


