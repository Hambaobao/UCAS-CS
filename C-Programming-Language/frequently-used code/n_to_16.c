#include <stdio.h>

#define MAXLEN 32
#define MAXLENH 10

void transfer_to_2(int n, char bin[MAXLEN]);

void transfer2_to_16(char bin[MAXLEN], char hex[10]);

void anticode(char bin[]);

void completecode(char bin[]);

int main(){
	int n;
	char bin[MAXLEN];
	char hex[10];
	scanf("%d",&n);
	if (n>=0)
		{
			transfer_to_2(n,bin);
			transfer2_to_16(bin,hex);
							}
	else
		{
			n=-1*n;
			transfer_to_2(n,bin);
			anticode(bin);
			completecode(bin);
			transfer2_to_16(bin,hex);
							}
	
	for (int i=0;i<MAXLENH;i++)
		{
			printf("%c",hex[i]);
						}
		printf("\n");
	return 0;
}

void transfer_to_2(int n, char bin[MAXLEN]){	
	for (int i=0;i<MAXLEN;i++)
		{
			bin[i]='0';
					}
	for (int j=1;n>0;j++)
		{
			bin[MAXLEN-j]=n%2+'0';
			n=n/2;
		}
}

void anticode(char bin[MAXLEN]){	
	for (int i=0;i<MAXLEN;i++)
		{	
			if (bin[i]=='0')
				bin[i]='1';
			else
				bin[i]='0';
		}	
}

void completecode(char bin[MAXLEN]){
	int i;	
	for (i=31;i>=-1&&bin[i]=='1';i--)
			;
	
	if (i==31)
		bin[i]='1';
	else if (i>=-1)
		{	
			bin[i]='1';
			for (int j=i+1;j<MAXLEN;j++)
				bin[j]='0';
					}
}

void transfer2_to_16(char bin[MAXLEN], char hex[10]){
	int i,j;
	for (i=0,j=2;i<MAXLEN;i+=4,j++)
		{
			if 	(bin[i]=='0'&&bin[i+1]=='0'&&bin[i+2]=='0'&&bin[i+3]=='0')
					hex[j]='0';
			else if (bin[i]=='0'&&bin[i+1]=='0'&&bin[i+2]=='0'&&bin[i+3]=='1')
					hex[j]='1';
			else if (bin[i]=='0'&&bin[i+1]=='0'&&bin[i+2]=='1'&&bin[i+3]=='0')
					hex[j]='2';				
			else if (bin[i]=='0'&&bin[i+1]=='0'&&bin[i+2]=='1'&&bin[i+3]=='1')
					hex[j]='3';
			else if (bin[i]=='0'&&bin[i+1]=='1'&&bin[i+2]=='0'&&bin[i+3]=='0')
					hex[j]='4';
			else if (bin[i]=='0'&&bin[i+1]=='1'&&bin[i+2]=='0'&&bin[i+3]=='1')
					hex[j]='5';				
			else if (bin[i]=='0'&&bin[i+1]=='1'&&bin[i+2]=='1'&&bin[i+3]=='0')
					hex[j]='6';
			else if (bin[i]=='0'&&bin[i+1]=='1'&&bin[i+2]=='1'&&bin[i+3]=='1')
					hex[j]='7';
			else if (bin[i]=='1'&&bin[i+1]=='0'&&bin[i+2]=='0'&&bin[i+3]=='0')
					hex[j]='8';				
			else if (bin[i]=='1'&&bin[i+1]=='0'&&bin[i+2]=='0'&&bin[i+3]=='1')
					hex[j]='9';
			else if (bin[i]=='1'&&bin[i+1]=='0'&&bin[i+2]=='1'&&bin[i+3]=='0')
					hex[j]='A';
			else if (bin[i]=='1'&&bin[i+1]=='0'&&bin[i+2]=='1'&&bin[i+3]=='1')
					hex[j]='B';				
			else if (bin[i]=='1'&&bin[i+1]=='1'&&bin[i+2]=='0'&&bin[i+3]=='0')
					hex[j]='C';
			else if (bin[i]=='1'&&bin[i+1]=='1'&&bin[i+2]=='0'&&bin[i+3]=='1')
					hex[j]='D';
			else if (bin[i]=='1'&&bin[i+1]=='1'&&bin[i+2]=='1'&&bin[i+3]=='0')
					hex[j]='E';				
			else if (bin[i]=='1'&&bin[i+1]=='1'&&bin[i+2]=='1'&&bin[i+3]=='1') 
					hex[j]='F';
			else
				;
		}
	hex[0]='0';
	hex[1]='x';	
}
			




























