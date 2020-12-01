#include <stdio.h>

#define MAXLEN 100

int mygetline(char s[],int lim);
int judge(char s[MAXLEN],int len);

int main(){
	int len;
	char s[MAXLEN],c;
	do{
		len=mygetline(s,MAXLEN);
		printf("%d",judge(s,len));
	}while(c=getchar()!=EOF);	

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

int judge(char s[MAXLEN],int len){
	int i=0;
	int p=0;
	char temp[len];
	if((len-1)%2==0){
		int q=(len-1)/2;
		int j=1;
		while(s[q-1-j]==s[q-1+j]&&(q-1-j)>=0)
			j++;
		return ((q-1-j)==-1)?1:0;
	}

	else{
		if(s[i]=='@')
			return 1;
		else
			temp[p]=s[i];
		i++;
	
		while(s[i]!='@'){
			if(s[i]!=temp[p]){
				p++;
				temp[p]=s[i];
			}
			else{
				p--;
			}
		i++;		
		}
		return (p==-1)?1:0;
	}
}
