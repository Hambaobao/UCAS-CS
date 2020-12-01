viod itoa(int n, char s[]){
	void reverse(char s[]);
	int i,sign;
	sign=n;
	if (n<0)
		n=-n;
	i=0;
	do{
		s[i++]=n%10+'0';
		n/=10;
			} while(n>0);
	if (sign<0)
		s[i++]='-';
	s[i]='\0';
	reverse (s);
}
