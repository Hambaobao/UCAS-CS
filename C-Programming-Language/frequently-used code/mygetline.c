//version 1
int mygetline(char input[],int lim)
{	int i;
	char c;
	for(i=0;i<=lim-1&&(c=getchar())!=EOF&&c!='\n';++i)
		input[i]=c;
	if(c=='\n')
	{	input[i]=c;
		++i;
	}
	input[i]='\0';
	return i;
}


//version 2
int mygetline(char s[],int lim)
{	int i,c;
	i=0;
	while(--lim>0 && (c=getchar())!=EOF && c!='\n')
		s[i++]=c;
	if(c=='\n')
		s[i++]='\n';
	s[i]='\0';
	return i;
}
