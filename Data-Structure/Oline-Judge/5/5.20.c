#include<stdio.h>
#define NOTDONE 0
#define DONE 1
#define POS 0
#define NEG 1

int main()
{
	char c;
	int tempnum=0;
	int sign=POS;
	int dimension=0;
	int length;
	int head=NOTDONE;
	int matrix[1000];
	int totalnum=0;
	int matrix_i;
	int flag=NOTDONE;
	int va[10];
	
	int va_i=0;
	int divident=0;
	int output_j=0;
	for(int i=0;i<1000;i++)
		matrix[i]=0;
	for(int i=0;i<10;i++)
		va[i]=0;




	while((c=getchar())!=EOF){
		if(head==NOTDONE)
		{	if(c>='0'&&c<='9')
				tempnum=tempnum*10+c-'0';
			if(c==';'&&dimension==0)
				{dimension=tempnum;tempnum=0;}
			else if(c==';')
				{length=tempnum;tempnum=0; head=DONE;}
		}
		else{
			if(c>='0'&&c<='9')
				tempnum=tempnum*10+c-'0';
			if(c==',')
			{	if(sign==NEG)
					{matrix[totalnum++]=-tempnum;sign=POS;}
				else
					matrix[totalnum++]=tempnum;
				
				tempnum=0;
				
			}
		 	if(c==';')
		 		{	if(sign==NEG)
		 				matrix[totalnum++]=-tempnum;
		 			else
		 				matrix[totalnum++]=tempnum;
		 			tempnum=0;
		 			sign=POS;
		 		}

		 	if(c=='-')
		 		sign=NEG;




		}
}
	if(sign==NEG)
		 matrix[totalnum++]=-tempnum;
	else
		matrix[totalnum++]=tempnum;

	

/*--------------------above is input------------------*/
	
	for(matrix_i=totalnum-1;matrix_i>=0;matrix_i--)
	{
		if(matrix[matrix_i]==0)continue;
		else{
			if(flag==NOTDONE)
			{
				if(matrix[matrix_i]==1)
					   ;
				else if(matrix[matrix_i]==-1)
					printf("-");
				else printf("%d",matrix[matrix_i]);
			}
			else{
				if(matrix[matrix_i]==1)
					printf("+");   
				else if(matrix[matrix_i]==-1)
					printf("-");
				else if(matrix[matrix_i]>0)
					printf("+%d",matrix[matrix_i]);
				else printf("%d",matrix[matrix_i]);

			}



			flag=DONE;
			}
	
/*---------------------coeficient------------------*/
		for(va_i=0,divident=matrix_i;va_i<dimension;va_i++)
		{	
			va[va_i]=divident%length;
			divident=divident/length;

		}

		
		for(output_j=1,va_i--;output_j<=dimension;output_j++,va_i--)
		{  


			

			if(va[va_i]!=0&&va[va_i]!=1)
				printf("x%dE%d",output_j,va[va_i]);
			else if(va[va_i]==1)
				printf("x%d",output_j);
			

		}



	}

		if(matrix[0]==1)
			printf("1");



	return 0;
}