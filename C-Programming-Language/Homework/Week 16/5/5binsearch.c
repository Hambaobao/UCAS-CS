#include "5myhead.h"
#include <string.h>

int binsearch(char *word, struct key tab[], int n){
	int cond;
	int low,high,mid;
	low=0;
	high=n-1;
	while(low<=high)
	{
		mid=(high+low)/2;
		if ((cond=strcmp(word,tab[mid].word))<0)
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
	return -1;
}
