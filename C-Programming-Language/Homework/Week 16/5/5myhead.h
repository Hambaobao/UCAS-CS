#ifndef _MY_HEAD_H
#define _MY_HEAD_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key))

struct key{
	char *word;
	int count;
};

int getword(char *,int);
int binsearch(char *,struct key *,int);

#endif
