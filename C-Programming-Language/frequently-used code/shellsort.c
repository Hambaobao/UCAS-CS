int shellsort(int s[],int len){
	int i,j,gap;
	int temp;
	for (gap=len/2;gap>0;gap/=2)
		{
		for (i=gap;i<len;++i){
			for (j=i-gap;j>=0 && s[j]>s[j+gap];j-=gap){
				temp=s[j+gap];
				s[j+gap]=s[j];
				s[j]=temp;
					}
			}
	}
}
