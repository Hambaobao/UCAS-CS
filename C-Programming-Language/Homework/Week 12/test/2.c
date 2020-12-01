#include "3.h"

double stack[STACKSIZE];
int sp = 0;

void push(double x)
{
	if (sp >= STACKSIZE)
	{
		printf("Error: The stack is too full to push %g\n", x);
		error = 1;
	}
	else
		stack[sp++] = x;
}

double pop(void)
{
	if (sp == 0)
	{
		printf("Error: The stack is empty!\n");
		error = 1;
		return 0.0;
	}
	else
		return stack[--sp];
}
