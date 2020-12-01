#include <stdio.h>
int mystrcmp(char *s,char *t);
int main(){
	printf("%d\n",mystrcmp("hello kitty","hello kitty"));
	printf("%d\n",mystrcmp("hello kitty1","hello kitty2"));
	printf("%d\n",mystrcmp("hello kitty","Hello kitty"));
	return 0;
}

int mystrcmp(char *s, char *t){
	int i;
	for (i=0;s[i]==t[i];i++)
		if(s[i]=='\0')
			return 0;
	return s[i]-t[i];
}
