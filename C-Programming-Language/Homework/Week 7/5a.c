#include <ctype.h>

int atoi(char s[]){
	int i,n,sign;
	for (i=0;isspace(s[i]);++i)//跳过所有空白符
		;
	sign=(s[i]=='-')?-1:1;//判断第一个非空格字符是否为-号
	if (s[i]=='-'||s[i]=='+')//跳过符号
		++i;
	for (n=0;isdigit(s[i]);++i)
		n=n*10+(s[i]-'0');
	return sign*n;
}
