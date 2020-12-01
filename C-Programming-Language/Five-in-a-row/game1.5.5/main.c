#include "head.h"

int main()
{
	char model,c;
	//”Œœ∑≥ı ºªØ
	system("clear");
	printf("choose model:\n");
	printf("input 1 : player vs player\n");printf("input 2 : player vs computer(black)\n");printf("input 3 : player vs computer(white)\n");printf("input 4 : test model\n");
	scanf("%c",&model);

	initRecordBorard();
	recordtoDisplayArray();
	displayBoard();		
	printf("-----game start-----\n");
	printf("input q to quilt, and others to continue:");	
	c=getchar();
	if(c=='q'||c=='Q'||c==EOF)
		return 0;
	else
		;
	switch(model)
	{
		case '1': playersvs();break;
//		case '2': player_vs_computer_black();break;
//		case '3': player_vs_computer_white();break;
		case '4': test();break;
	}
	

	return 0;
}

