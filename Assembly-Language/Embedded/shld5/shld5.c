#include <stdio.h>

unsigned int shld5(unsigned int a, unsigned int b)
{    
	unsigned int result;    

//	result = (a << 5) |( b>>(32-5)); //此语句用嵌入式汇编编写

	asm(
		"movl	%2, %%eax\n\t"
		"movl	%1, %%ebx\n\t"
		"shll	%%eax, 5\n\t"
		"shrl	%%ebx, 27\n\t"
		"orl	%%ebx, %%eax\n\t"
		:"=a"(result)
		:"b"(b),"a"(a)
	);

	return result;
}
