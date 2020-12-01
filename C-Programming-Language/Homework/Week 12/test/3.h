#include <stdio.h>
#include <stdlib.h>  // in order to use the func atof
#include <ctype.h>

#define MAXLEN 1000
#define STACKSIZE 1000
#define BUFSIZE 1000
#define NUMBER '0'

int getop(char s[]);
void push(double);
double pop(void);
void Initial(void);
int getch(void);
void ungetch(int c);

extern int error;
extern int sp;
extern int bufp;
