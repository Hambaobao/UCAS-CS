#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HASHSIZE 101

struct node
{
	struct node *next;
	char *name;
	char *defn;
};

static struct node *hashtab[HASHSIZE];
struct node *install(char *name,char *defn);
struct node *lookup(char *s);
unsigned hash(char *s);
void buildlist();
char *mystrdup(char *s);
