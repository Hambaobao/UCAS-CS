#include <stdio.h>
#include <string.h>

#define MAXLEN 100
int mygetline(char s[],int lim);
int kmp(char *s, char *p);
void getnext(char *p,int next[]);

int next[MAXLEN];

int main(){
	char input[MAXLEN];
	char sub[MAXLEN];

	int leninput,lensub;
	char *p;

	leninput = mygetline(input,MAXLEN);
	lensub = mygetline(sub,MAXLEN);
	getnext(sub,next);
	

	printf("%d",kmp(input,sub));	
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
	return i-1;
}

int kmp(char *s, char *p){
	int i=0,j=0;
	int slen = strlen(s) -1;
	int plen = strlen(p) -1;
	while(i<slen && j<plen){
		if(j == -1 || s[i] == p[j])
		{
			i++;
			j++;	
		}
		else
		{
			j = next[j];
		}
	}
	if(j == plen)
		return i-j;
	else
		return -1;
}

void getnext(char *p,int next[]){
	int plen = strlen(p);
	next[0] = -1;
	int k=-1;
	int j=0;
	while(j<plen){
		if(k == -1 || p[j] == p[k]){
			j++;
			k++;
		
			if(p[j] != p[k])
				next[j] = k;
			else
				next[j] = next[k];
		}
		else
			k = next[k];			
	}

}





