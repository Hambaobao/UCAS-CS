#include <stdio.h>
struct string{
	int len;
	char *str;
}*p;
char odd[]="13579";
char alpha[]="aceghjlnpr";
int main(){
	struct string s[2]={
		{5,odd},
		{10,alpha}
	};
	p=s;
	printf("p=%p",p); printf(" p->len: %d\n",p->len);
	printf("p=%p",p); printf(" ++p->len: %d\n",++p->len);
	printf("p=%p",p); printf(" (++p)->len: %d\n",(++p)->len);
	printf("p=%p",p); printf(" p->len: %d\n",p->len);
	p=s;
	printf("p=%p",p); printf(" p->str: %s\n",p->str);
	printf("p=%p",p); printf(" *p->str++: %c\n",*p->str++);
	printf("p=%p",p); printf(" *p->str: %c\n",*p->str);
	printf("p=%p",p); printf(" (*p->str)++: %c\n",(*p->str)++);
	printf("p=%p",p); printf(" (*p->str): %c\n",(*p->str));
	printf("p=%p",p); printf(" *p++->str: %c\n",*p++->str);
	printf("p=%p",p); printf(" *p->str: %c\n",*p->str);
	return 0;
}
