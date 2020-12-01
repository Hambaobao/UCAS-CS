#include "dcl.h"
int gettoken(void){
	int c;
	char *p=token;
	while((c=getch())==' ' || c=='\t')
		;
	if(c=='('){
		if((c=getch())==')'){
			strcpy(token,"()");
			return tokentype=PARENS;
		}else{
			ungetch(c);
			return tokentype='(';
		}
	}else if(c=='['){
		for(*p++=c;(*p++=getch())!=']';)
			;
		*p='\0';
		return tokentype=BRACKETS;
	}else if(isalpha(c)){
		for(*p++=c;isalnum(c=getch());)
			*p++=c;
		*p='\0';
		ungetch(c);
		return tokentype=NAME;
	}else
		return tokentype=c;
}
