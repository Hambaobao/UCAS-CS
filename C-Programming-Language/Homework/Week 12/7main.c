#include <stdio.h>
#include <string.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];
int readlines(char *lineptr[],int nlines);
void writelines(char *lineptr[],int nlines);
void qsort(char *lineptr[],int left,int right);
int main(){
	int nlines;
	if((nlines=readlines(lineptr,MAXLINES))>=0){
		qsort(lineptr,0,nlines-1);
		printf("After Sorting......\n");
		writelines(lineptr,nlines);
		return 0;	
	}else{
		printf("error: input too gig to sort\n");
		return 1;
	}
}


