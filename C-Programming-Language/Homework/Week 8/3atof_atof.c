#include <ctype.h>

double atof(char s[]){
	double num,power;
	int i,sign;
	for(i=0;isspace(s[i]);i++)
		;
	sign=(s[i]=='-')?-1:1;
	if(s[i]=='-'||s[i]=='+')
		i++;
	for(num=0.0;isdigit(s[i]);i++)
		num=10.0*num+s[i]-'0';
	if(s[i]=='.')
		i++;
	for(power=1.0;isdigit(s[i]);i++)
		{
			num=10.0*num+s[i]-'0';
			power*=10.0;
					}
	return sign*(num/power);
}

