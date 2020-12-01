#include "3.h"

int error = 0; // when error occurs(like invalid input), this item will become 1

int main(void)
{
	int type;
	double op2;
        int a,b,c;
	char s[MAXLEN];  //the array s is used to store data to be transformed

	while ((type = getop(s)) != EOF)
	{
		switch (type)
		{
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '&':
                        a=pop();
                        b=pop();
                        a&b;c=b&a;
			push(c);
			break;
		case '|':
                        a=pop();
                        b=pop();
                        a|b;c=b|a;
			push(c);
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
                case '%':
                        a=pop();
                        b=pop();
                        c=b%a;
			push(c);
			break;
		case '*':
			push(pop() * pop());
			break;
		case '/':
			op2 = pop();
			if (op2 == 0.0)
				printf("Error: zero can't be used as divisor!\n");
			else
				push(pop() / op2);
			break;
		case '\n':
			if(error){
				printf("Error: Invalid input!\n");
                                main();  }
			else
			{
				if(sp != 1){
					printf("Error: Inadequate operators!\n");
                                        main();}
				else
					printf("The result is %.8g\n", pop());
			}
			
			Initial;			
			break;
		default:
			printf("Error: Invalid input! Please input again!\n");
			break;
		}
	}

	return 0;
}

void Initial(void)
{
	error = 0;
//	sp = 0;
//	bufp = 0;
}
