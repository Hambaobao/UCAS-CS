#include "1myhead.h"
#include <string.h>

struct key* binsearch(char *word, struct key* tab, int n){
	int cond;
	struct key* low=&tab[0];
	struct key* high=&tab[n-1];
	struct key* mid;

	while(low<=high)
	{
		mid=low+(high-low)/2;
		if ((cond=strcmp(word,mid->word))<0)
		{
			high=mid-1;
		}
		else if (cond>0)
		{
			low=mid+1;
		}
		else 
		{
			return mid;
		}
	}
	return NULL;
}
