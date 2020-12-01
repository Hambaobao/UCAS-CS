#include <stdio.h>
#include <string.h>

#define MAXLEN 100
int mygetline(char s[],int lim);
int kmp(char *s, char *p);
void getnext(char *p,int next[]);
void copysub(char *p,char *s,int len);
int search(char *input,int len);

int next[MAXLEN];
char finalsub[MAXLEN];

int main(){
	char input[MAXLEN];
	char sub[MAXLEN];
	int leninput;
	char *p = input;
	int loc;
	leninput = mygetline(input,MAXLEN);
	loc = search(input,leninput);

	if(loc!=-1){
		printf("%s ",finalsub);
		printf("%d",loc);
	}else
		printf("%d",-1);	
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
	int slen = strlen(s);
	int plen = strlen(p);
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

int search(char *input,int len){
	int length;
	int res;
	char pre[len];

	for(length=((len%2)?((len+1)/2):(len/2));length>0;length--){
		int loc=0;
		while((loc+2*length)<=len){
			copysub(input+len-length,finalsub,length);
			getnext(finalsub,next);
			copysub(input,pre,(len-length));
			res=kmp(pre,finalsub);
			if(res!=-1)
				return res;
			else
				loc++;
		}	
	}
	return -1;		
}

void copysub(char *p,char *s,int len){
	int i;
	for(i=0;i<len;i++)
		*(s+i)=*(p+i);
	*(s+i)='\0';
}




