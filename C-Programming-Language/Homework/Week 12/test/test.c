#include <stdio.h>
#include <stdlib.h>

int main(){
	char a;
	int n,m;
	int l;
	scanf("%d",&n);
	scanf("%d",&m);
	l=(m&n);
	printf("n=%d\n",n);
	printf("m=%d\n",m);
	printf("m&n=%d\n",l);

	/*scanf("%c",&a);
	n=a-'a';
	printf("%c\n",a);
	printf("%d\n",a);
	printf("%d\n",n);
	*/
	return 0;
}
