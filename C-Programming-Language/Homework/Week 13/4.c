#include <stdio.h>

int main(){

	static char name1[13][15]={
	"Illegal month","January","February","March","April","May","June","July","August","September","October","November","December"
	};

	static char *name2[13]={
	"Illegal month","January","February","March","April","May","June","July","August","September","October","November","December"
	};

	printf("name1[1][0]=%c\n",name1[1][0]);
	printf("name2[1][0]=%c\n",name2[1][0]);

	name2[1]=name2[2];

	printf("name2[1][0]=%c\n",name2[1][0]);
	printf("len of name1=%3ld elements totalbytes=%3d bytes\n",sizeof(name1)/sizeof(char),sizeof(name1));
	printf("len of name2=%3ld elements totalbytes=%3d bytes\n",sizeof(name2)/sizeof(char*),sizeof(name2));

	return 0;
}
