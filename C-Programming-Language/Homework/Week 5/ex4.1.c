#include <stdio.h>
enum escapes{
	BELL='\a',BACKSPACE='\b',TAB='\t',NEWLINE='\n',VTAB='\v',RETURN='\r'};
int main(){
	enum escapes be,ba,ta,ne,vt,re;
	be=BELL;
	ba=BACKSPACE;
	ta=TAB;
	ne=NEWLINE;
	vt=VTAB;
	re=RETURN;
	printf("be=%d\n",be);
	printf("ba=%d\n",ba);
	printf("ta=%d\n",ta);
	printf("ne=%d\n",ne);
	printf("vt=%d\n",vt);
	printf("re=%d\n",re);
	return 0;
}
