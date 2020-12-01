int atoi (char s[]){
	int i,n,sign;
	for(i=0;isspace(s[i]);i++)//跳过空白符;
		sign=(s[i]=='-')?-1:1;
	if(s[i]=='+'||s[i]=='-')//跳过符号
  		i++;
	for(n=0;isdigit(s[i]);i++)
	       n=10*n+(s[i]-'0');//将数字字符转换成整形数字
	return sign *n;
}
