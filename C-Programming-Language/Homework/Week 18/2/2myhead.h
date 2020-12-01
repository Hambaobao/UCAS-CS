#ifndef _MY_HEAD_H
#define _MY_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXWORD 100
#define BUFLEN 10000

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
extern char filebuf[BUFLEN];
extern int size;

#endif