#include <stdio.h>

#define MAXLEN 50

int mygetline(char s[],int lim);
void change(char input[MAXLEN],char output[MAXLEN],int len);
int isoptr(char a);
int isopnd(char a);
int inputbiger(char input,char s);

int main(){
	char input[MAXLEN];
	char output[MAXLEN];
	int len;
	len = mygetline(input,MAXLEN);
	change(input,output,len);
	printf("%s",output);
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

void change(char input[MAXLEN],char output[MAXLEN],int len){
	char s1[MAXLEN];
	char s2[MAXLEN];
	int p1=0;
	int p2=0;
	s1[0]='#';
	
	for(int i=0;i<len;i++){
		if(isopnd(input[i]))
			s2[p2++]=input[i];
		else if(isoptr(input[i])){
			if(s1[p1]=='#' || s1[p1]=='(')			//1
				s1[++p1]=input[i];
			else if(inputbiger(input[i],s1[p1]))		//2
				s1[++p1]=input[i];
			else if(!inputbiger(input[i],s1[p1])){		//3
					while(!(inputbiger(input[i],s1[p1]) || s1[p1]=='#')){
						s2[p2++]=s1[p1--];
					}

					s1[++p1]=input[i];
				}
			}
		else if(input[i]=='(')
				s1[++p1]=input[i];
		else if(input[i]==')'){
			while(s1[p1]!='('){
				s2[p2++]=s1[p1--];	
				}
			p1--;continue;
		}			
	}
	
	for(int i=p1;s1[i]!='#';i--)
		s2[p2++]=s1[i];

	for(int i=0;i<p2;i++)
		output[i]=s2[i];
	output[p2]='\0';
	
}

int isoptr(char a){
	return (a == '+' || a == '-' || a == '*' || a == '/' || a == '#')?1:0;
}

int isopnd(char a){
	if(a>='a'&& a<='z')
		return 1;
	if(a>='A' && a<='Z')
		return 1;
	return 0;
}

int inputbiger(char input,char s){
	if((input=='*'||input=='/') && (s=='+' || s=='-' || s=='(' || s=='#'))
		return 1;
	else if((input=='+' || input =='-') && (s=='(' || s=='#'))
		return 1;
	else if(input=='(')
		return 0;
	else if(input==')')
		return 1;
	else
		return 0;
}


























