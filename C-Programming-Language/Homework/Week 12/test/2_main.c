#include <stdio.h>
#include <stdlib.h> //为了使用atof函数
#define MAXOP 100   //操作数或运算符的最大长度
#define NUMBER '0'  //标示找到一个数

#define MAXLEN 1000

int getop(char []);
void push(double );
double pop(void);

extern int sp;
extern int error;

int r=0;
char input[MAXLEN];


//逆波兰计算器
int main(){
	int type;
	double op2;
	int op3;
	int op4;
	int k;
	char s[MAXOP];
	while ((type=getop(s))!=EOF){
		if(type=='0')
			input[r++]=atof(s)+'0';
		else
			input[r++]=type;
		switch(type){
			case NUMBER:
				push(atof(s));break;
			case '+':
				push(pop()+pop());break;
			case '*':
				push(pop()*pop());break;
			case '-':
				op2=pop();
				push(pop()-op2);break;
			case '/':
				op2=pop();
				if(op2!=0.0)
					push(pop()/op2);
				else
					printf("error: zero divisor\n");
				break;
			case '%':
				op3=pop();
				op4=pop();
				push(op4%op3);break;
			case '&':
				op3=pop();
				op4=pop();
				push(op4&op3);break;
			case '|':
				op3=pop();
				op4=pop();
				push(op4|op3);break;
/*			case '<':
				if(((op3=pop())=='<'))
				{
					op4=pop();
					push(op4<<op3);break;
				}
				else
				{
					op4=pop();
					if(op4<op3)
						{push(1);break;}
					else
						{push(0);break;}	
				}
			case '>':
				if(((op3=pop())=='>'))
				{
					op4=pop();
					push(op4>>op3);break;
				}
				else
				{
					op4=pop();
					if(op4>op3)
						{push(1);break;}
					else
						{push(0);break;}	
				}						*/
			case '\n':
				if(error==1)
				{
					printf("inputs error, please input again\n");
					sp=0;
					error=0;
					main();
				}
				else 
				{
					if (sp!=1)
					{
						printf("inputs error, please input again\n");
						sp=0;
						error=0;
						main();
					}
					else
					{
						printf("The result is %.8g\n", pop());
			//			printf("%s",input);
						break;
					}
				}					 
			default  :
				printf("error: unknown command %s\n",s);break;
			}
	}
	return 0;
}








