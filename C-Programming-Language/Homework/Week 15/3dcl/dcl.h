#ifndef _DCL_H
#define _DCL_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXTOKEN 100
enum{NAME,PARENS,BRACKETS};
extern char out[];
extern char token[];
extern char name[];
extern char datatype[];
extern int tokentype;

void dcl(void);
void dirdcl(void);
int gettoken(void);
#endif
