#include <stdio.h>

int strindex(char s[],char t[]){
	int i=0;
	int j,k;
	for(i=0;s[i]!='\0';i++)
		{
			{
				for(j=i,k=0;t[k]!='\0'&&s[j]==t[k];j++,k++)
						;				
							}
			if(k>0 && t[k]=='\0')
				return i;
		}
	return -1;			
}
