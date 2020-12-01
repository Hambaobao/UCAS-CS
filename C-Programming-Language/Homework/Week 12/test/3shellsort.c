#include <string.h>

int shellsort(char *s[],int len){
	int i,j,gap;
	char *temp;
	for (gap=len/2;gap>0;gap/=2)
		{
		for (i=gap;i<len;++i){
			for (j=i-gap;j>=0 && (strcmp(s[j+gap],s[j])<0);j-=gap){
				temp=s[j+gap];
				s[j+gap]=s[j];
				s[j]=temp;
					}
			}
	}
}
