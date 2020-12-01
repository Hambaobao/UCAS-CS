#include "dcl.h"

void dcl(void){
	int ns;
	for(ns=0;gettoken()=='*';)
		ns++;
	dirdcl();
	while(ns-->0)
		strcat(out," pointer to");
}

void dirdcl(void){
	int type;
	if(tokentype=='('){
		dcl();
		if(tokentype!=')')
			printf("error: missing)\n");
	}else if(tokentype==NAME)
		strcpy(name,token);
	else
		printf("error: expected name or (dcl)\n");
	while((type=gettoken())==PARENS || type==BRACKETS)
		if(type==PARENS){
		strcat(out," function returing");	
	}else{
		strcat(out," array");
		strcat(out,token);
		strcat(out," of");
	}
}
