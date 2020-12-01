#include "3myhead.h"

void buildlist(void){
	struct name_and_defn{
		char *name;
		char *defn;
	};
	struct name_and_defn ndpairs[]={
		{"IN","1"},{"OUT","2"},{"MAX","10000"},
		{"ZHANGSAN","1"},{"LISI","2"},{"MIN","3"},{"NUMBER","1024"}
	};
	int i;

	for(i=0;i<sizeof(ndpairs)/sizeof(struct name_and_defn);i++)
		install(ndpairs[i].name,ndpairs[i].defn);
	return;
}

struct node *install(char *name ,char *defn){
	struct node *np;
	unsigned hashval;
	if((np=lookup(name))==NULL){
		np=(struct node *)malloc(sizeof(*np));
		if(np==NULL||(np->name = strdup(name))==NULL)
			return NULL;
		hashval=hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval]=np;
	}else
		free((void*)np->defn);
	if ((np->defn=mystrdup(defn))==NULL)
		{
			return NULL;
		}
	return np;		
}

unsigned hash(char *s){
	unsigned hashval;
	for(hashval=0;*s!='\0';s++)
		hashval=*s+31*hashval;
	return hashval%HASHSIZE;
}


char *mystrdup(char *s){
	char *p;
	p=(char*)malloc(strlen(s)+1);
	if(p!=NULL)
		strcpy(p,s);
	return p;
}

struct node *lookup(char *s){
	struct node *np;
	for(np=hashtab[hash(s)];np!=NULL;np=np->next)
		if(strcmp(s,np->name)==0)
			return np;
	return NULL;	
}
