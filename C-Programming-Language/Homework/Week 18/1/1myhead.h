#ifndef _MY_HEAD_H
#define _MY_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXWORD 100

struct tnode{
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

char *mystrdup(char *s);
struct tnode * talloc(void);
struct tnode * addtree(struct tnode * ptree,char *word);
void treeprint(struct tnode * ptree);
int getword(char *word,int lim);

#endif
