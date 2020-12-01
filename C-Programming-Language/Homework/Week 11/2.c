#include <stdio.h>
int a;
int *pb;

int main(){
	int b;
	int *pa;
	pa=&a;
	pb=&b;
	printf("addr_of_a =%p\n",&a);	
	printf("addr_of_b =%p\n",&b);
	printf("data_in_pa=%p\n",pa);
	printf("data_in_pb=%p\n",pb);
	printf("the size of pa=%ld\n",sizeof(pa));
	printf("the size of pb=%ld\n",sizeof(pb));
	printf("addr_of_pa=%p\n",&pa);
	printf("addr_of_pb=%p\n",&pb);
	return 0;
}
